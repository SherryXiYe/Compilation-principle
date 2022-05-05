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

const int	NUM_THREADS=3;
int N=0;
float** m;
//信号量定义
sem_t sem_main;
sem_t sem_workerstart[NUM_THREADS]; // 每个线程有自己专属的信号量
sem_t sem_workerend[NUM_THREADS];

typedef struct{
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

void *threadFunc(void *param)  //按行循环划分
{
    threadParam_t *p = (threadParam_t*)param;
    int t_id=p->t_id;   //线程编号
    for(int k=0;k<N;k++)
    {
        sem_wait(&sem_workerstart[t_id]); //阻塞，等待主线完成除法操作（操作自己专属的信号量）
        for(int i=k+1+t_id;i<N;i+=NUM_THREADS)
        {
            for(int j=k+1;j<N;j++)
                m[i][j]=m[i][j]-m[i][k]*m[k][j];
            m[i][k]=0;
        }
        sem_post(&sem_main);//唤醒主线程
        sem_wait(&sem_workerend[t_id]); //阻塞，等待主线程唤醒进入下一轮
    }
    pthread_exit(NULL);
}
void *threadFunc_1(void *param)  //按行进行块划分
{
    threadParam_t *p = (threadParam_t*)param;
    int t_id=p->t_id;   //线程编号
    for(int k=0;k<N;k++)
    {
        sem_wait(&sem_workerstart[t_id]); //阻塞，等待主线完成除法操作（操作自己专属的信号量）
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
        sem_post(&sem_main);//唤醒主线程
        sem_wait(&sem_workerend[t_id]); //阻塞，等待主线程唤醒进入下一轮
    }
    pthread_exit(NULL);
}
void *threadFunc_2(void *param)  //按列进行块划分
{
    threadParam_t *p = (threadParam_t*)param;
    int t_id=p->t_id;   //线程编号
    for(int k=0;k<N;k++)
    {
        sem_wait(&sem_workerstart[t_id]); //阻塞，等待主线完成除法操作（操作自己专属的信号量）
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
        sem_post(&sem_main);//唤醒主线程
        sem_wait(&sem_workerend[t_id]); //阻塞，等待主线程唤醒进入下一轮
    }
    pthread_exit(NULL);
}
void *threadFunc_3(void *param)  //按列进行循环划分
{
    threadParam_t *p = (threadParam_t*)param;
    int t_id=p->t_id;   //线程编号
    for(int k=0;k<N;k++)
    {
        sem_wait(&sem_workerstart[t_id]); //阻塞，等待主线完成除法操作（操作自己专属的信号量）
        for(int i=k+1;i<N;i++)
        {
            for(int j=k+1+t_id;j<N;j+=NUM_THREADS)
                m[i][j]=m[i][j]-m[i][k]*m[k][j];
            m[i][k]=0;
        }
        sem_post(&sem_main);//唤醒主线程
        sem_wait(&sem_workerend[t_id]); //阻塞，等待主线程唤醒进入下一轮
    }
    pthread_exit(NULL);
}
void *threadFunc_4(void *param)  //按行进行循环划分，采用对齐方式的SSE编程
{
    threadParam_t *p = (threadParam_t*)param;
    int t_id=p->t_id;   //线程编号
    __m128 vaik,vakj,vaij,vx;
    int j;
    for(int k=0;k<N;k++)
    {
        sem_wait(&sem_workerstart[t_id]); //阻塞，等待主线完成除法操作（操作自己专属的信号量）
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
        sem_post(&sem_main);//唤醒主线程
        sem_wait(&sem_workerend[t_id]); //阻塞，等待主线程唤醒进入下一轮
    }
    pthread_exit(NULL);
}
void *threadFunc_5(void *param)  //按行进行循环划分，采用对齐方式的AVX编程
{
    threadParam_t *p = (threadParam_t*)param;
    int t_id=p->t_id;   //线程编号
    __m256 vaik,vakj,vaij,vx;
    int j;
    for(int k=0;k<N;k++)
    {
        sem_wait(&sem_workerstart[t_id]); //阻塞，等待主线完成除法操作（操作自己专属的信号量）
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
        sem_post(&sem_main);//唤醒主线程
        sem_wait(&sem_workerend[t_id]); //阻塞，等待主线程唤醒进入下一轮
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
		sem_init(&sem_main,0,0);//初始化信号量
		for(j=0;j<NUM_THREADS;j++)
		{
            sem_init(&sem_workerstart[j],0,0);
            sem_init(&sem_workerend[j],0,0);
		}
		//创建线程
		pthread_t handles[NUM_THREADS];
        threadParam_t param[NUM_THREADS];
        for(id=0;id<NUM_THREADS;id++)
        {
            param[id].t_id=id;
            //pthread_create(&handles[id], NULL, threadFunc, (void *)&param[id]);    //按行进行循环划分
            pthread_create(&handles[id], NULL, threadFunc_1, (void *)&param[id]);  //按行进行块划分
            //pthread_create(&handles[id], NULL, threadFunc_2, (void *)&param[id]);  //按列进行块划分
            //pthread_create(&handles[id], NULL, threadFunc_3, (void *)&param[id]);  //按列进行循环划分
            //pthread_create(&handles[id], NULL, threadFunc_4, (void *)&param[id]);  //按行进行循环划分，采用对齐方式的SSE编程
            //pthread_create(&handles[id], NULL, threadFunc_5, (void *)&param[id]);    //按行进行循环划分，采用对齐方式的AVX编程

        }
		for(k=0;k<N;k++)
        {
            for(j=k+1;j<N;j++)    //主线程做除法操作
                m[k][j]=m[k][j]/m[k][k];
            m[k][k]=1.0;
            for(id=0;id<NUM_THREADS;id++)   //开始唤醒工作线程
                sem_post(&sem_workerstart[id]);
            for(id=0;id<NUM_THREADS;id++)  //主线程睡眠（等待所有的工作线程完成此轮消去任务）
                sem_wait(&sem_main);
            for(id=0;id<NUM_THREADS;id++)   //主线程再次唤醒工作线程进入下一轮次的消去任务
                sem_post(&sem_workerend[id]);
        }
        for(id=0;id<NUM_THREADS;id++)
            pthread_join(handles[id], NULL);
        sem_destroy(&sem_main);//销毁所有信号量
		for(j=0;j<NUM_THREADS;j++)
		{
            sem_destroy(&sem_workerstart[j]);
            sem_destroy(&sem_workerend[j]);
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
