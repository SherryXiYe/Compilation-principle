#include <iostream>
#include <nmmintrin.h>
#include <windows.h>
#include<stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <immintrin.h>
#include<malloc.h>
#include <stdio.h>
#include <stdint.h>
using namespace std;

int	NUM_THREADS=3;
int N=0;
float** m;

typedef struct{
    int k;   //消去的轮次
    int t_id;    //线程id
}threadParam_t;

void m_reset()
{
	m=new float*[N];
	for(int i=0;i<N;i++)
		m[i]=new float[N];
	for(int i=0;i<N;i++)
    {
        for(int j=0;j<i;j++)
			m[i][ j]=0;
		m[i][ i]=1.0;
		for(int j=i+1;j<N;j++)
			m[i][ j]=rand();
	}
	for(int k=0;k<N;k++)
		for(int i=k+1;i<N;i++)
			for(int j=0;j<N;j++)
				m[i][ j]+=m[k][j];
}
void m_reset_avx()
{
	m=(float **)_aligned_malloc( N* sizeof(float*),1024);
	for(int i=0;i<N;i++)
        m[i]=(float *)_aligned_malloc( N* sizeof(float),1024);
	for(int i=0;i<N;i++)
    {
        for(int j=0;j<i;j++)
			m[i][ j]=0;
		m[i][ i]=1.0;
		for(int j=i+1;j<N;j++)
			m[i][ j]=rand();
	}
	for(int k=0;k<N;k++)
		for(int i=k+1;i<N;i++)
			for(int j=0;j<N;j++)
				m[i][ j]+=m[k][j];
}
void seq()
{
	int i,j,k;
	for(k=0;k<N;k++)
	{
		for(j=k+1;j<N;j++)
			m[k][j]=m[k][j]/m[k][k];
		m[k][k]=1.0;
		for(i=k+1;i<N;i++)
		{
			for(j=k+1;j<N;j++)
				m[i][j]=m[i][j]-m[i][k]*m[k][j];
			m[i][k]=0;
		}
	}
}
void *threadFunc(void *param)  //按行循环划分
{
    threadParam_t *p = (threadParam_t*)param;
    int k=p->k;     //消去的伦次
    int t_id=p->t_id;   //线程编号
    for(int i=k+1+t_id;i<N;i+=NUM_THREADS)
    {
        for(int j=k+1;j<N;j++)
            m[i][j]=m[i][j]-m[i][k]*m[k][j];
        m[i][k]=0;
    }
    pthread_exit(NULL);
}
void *threadFunc_1(void *param)  //按行进行块划分
{
    threadParam_t *p = (threadParam_t*)param;
    int k=p->k;     //消去的伦次
    int t_id=p->t_id;   //线程编号
    int my_n = (N-k-1)/NUM_THREADS;
    int my_first = k+1+my_n*t_id;
    int my_last = my_first + my_n;
    if(my_last>N)
        my_last=N;
    for(int i=my_first;i<my_last;i++)
    {
        for(int j=k+1;j<N;j++)
            m[i][j]=m[i][j]-m[i][k]*m[k][j];
        m[i][k]=0;
    }
    pthread_exit(NULL);
}
void *threadFunc_2(void *param)  //按列进行块划分
{
    threadParam_t *p = (threadParam_t*)param;
    int k=p->k;     //消去的伦次
    int t_id=p->t_id;   //线程编号
    int my_n = (N-k-1)/NUM_THREADS;
    int my_first = k+1+my_n*t_id;
    int my_last = my_first + my_n;
    if(my_last>N)
        my_last=N;
    for(int i=k+1;i<N;i++)
    {
        for(int j=my_first;j<my_last;j++)
            m[i][j]=m[i][j]-m[i][k]*m[k][j];
        m[i][k]=0;
    }
    pthread_exit(NULL);
}
void *threadFunc_3(void *param)  //按列进行循环划分
{
    threadParam_t *p = (threadParam_t*)param;
    int k=p->k;     //消去的伦次
    int t_id=p->t_id;   //线程编号
    int my_n = (N-k-1)/NUM_THREADS;
    for(int i=k+1;i<N;i++)
    {
        for(int j=k+1+t_id;j<N;j+=NUM_THREADS)
            m[i][j]=m[i][j]-m[i][k]*m[k][j];
        m[i][k]=0;
    }
    pthread_exit(NULL);
}
void *threadFunc_4(void *param)  //按行进行循环划分，采用对齐方式的SSE编程
{
    threadParam_t *p = (threadParam_t*)param;
    int k=p->k;     //消去的伦次
    int t_id=p->t_id;   //线程编号
    __m128 vaik,vakj,vaij,vx;
    int j;
    for(int i=k+1+t_id;i<N;i+=NUM_THREADS)
    {
        vaik=_mm_set_ps(m[i][k],m[i][k],m[i][k],m[i][k]);
        for(j=k+1;(j%4!=0)&&j<N;j++)
        //Calculate the elements at the beginning of the line
        {
            m[i][j]=m[i][j]-m[i][k]*m[k][j];
        }
        for(;j+4<=N;j+=4)
        {
            vakj=_mm_load_ps(m[k]+j);
            vaij=_mm_load_ps(m[i]+j);
            vx=_mm_mul_ps(vakj,vaik);
            vaij=_mm_sub_ps(vaij,vx);
            _mm_store_ps(m[i]+j,vaij);
        }
        for(j=j-4;j<N;j++)
        //Calculates several elements at the end of the line
            m[i][j]=m[i][j]-m[i][k]*m[k][j];
        m[i][k]=0;
    }
    pthread_exit(NULL);
}
void *threadFunc_5(void *param)  //按行进行循环划分，采用对齐方式的AVX编程
{
    threadParam_t *p = (threadParam_t*)param;
    int k=p->k;     //消去的伦次
    int t_id=p->t_id;   //线程编号
    __m256 vaik,vakj,vaij,vx;
    int j;
    for(int i=k+1+t_id;i<N;i+=NUM_THREADS)
    {
        vaik=_mm256_set_ps(m[i][k],m[i][k],m[i][k],m[i][k],m[i][k],m[i][k],m[i][k],m[i][k]);
        for(j=k+1;(j%8!=0)&&j<N;j++)
        //Calculate the elements at the beginning of the line
        {
            m[i][j]=m[i][j]-m[i][k]*m[k][j];
        }
        for(;j+8<=N;j+=8)
        {
            vakj=_mm256_load_ps(m[k]+j);
            vaij=_mm256_load_ps(m[i]+j);
            vx=_mm256_mul_ps(vakj,vaik);
            vaij=_mm256_sub_ps(vaij,vx);
            _mm256_store_ps(m[i]+j,vaij);
        }
        for(j=j-8;j<N;j++)
        //Calculates several elements at the end of the line
            m[i][j]=m[i][j]-m[i][k]*m[k][j];
        m[i][k]=0;
    }
    pthread_exit(NULL);
}
int main()
{
	long long head, tail , freq ;
	int i;
	int n[10]={10,50,100,200,300,500,1000,2000,3000,4000};
	for(i=0;i<10;i++)
	{
		N=n[i];
		//m_reset_avx();
		m_reset();
        QueryPerformanceFrequency((LARGE_INTEGER*)&freq ) ;
        // start time
        QueryPerformanceCounter((LARGE_INTEGER* )&head) ;
		//to measure
		int k,j,id;
		for(k=0;k<N;k++)
        {
            for(j=k+1;j<N;j++)    //主线程做除法操作
                m[k][j]=m[k][j]/m[k][k];
            m[k][k]=1.0;
            //创建工作线程,进行消去操作
            pthread_t* handles=new pthread_t[NUM_THREADS];
            threadParam_t* param=new threadParam_t[NUM_THREADS];
            //分配任务
            for(id=0;id<NUM_THREADS;id++)
            {
                param[id].k=k;
                param[id].t_id=id;
            }
            //创建线程
            for(id=0;id<NUM_THREADS;id++)
            {
                //pthread_create(&handles[id], NULL, threadFunc, (void *)&param[id]);    //按行进行循环划分
                pthread_create(&handles[id], NULL, threadFunc_1, (void *)&param[id]);  //按行进行块划分
                //pthread_create(&handles[id], NULL, threadFunc_2, (void *)&param[id]);  //按列进行块划分
                //pthread_create(&handles[id], NULL, threadFunc_3, (void *)&param[id]);  //按列进行循环划分
                //pthread_create(&handles[id], NULL, threadFunc_4, (void *)&param[id]);  //按行进行循环划分，采用对齐方式的SSE编程
                //pthread_create(&handles[id], NULL, threadFunc_5, (void *)&param[id]);    //按行进行循环划分，采用对齐方式的AVX编程
            }
            //主线程挂起等待所有的工作线程完成此轮消去工作
            for(id=0;id<NUM_THREADS;id++)
                pthread_join(handles[id], NULL);
        }
		// end time
        QueryPerformanceCounter((LARGE_INTEGER *)&tail );
        cout <<(tail-head) * 1000.0 / freq << endl;
		for(int j=0;j<N;j++)
            delete[] m[j];
        delete[]m;
        /*for(int j=0;j<N;j++)        //avx
            _aligned_free(m[j]);
        _aligned_free(m);*/
	}
	return 0;
}
