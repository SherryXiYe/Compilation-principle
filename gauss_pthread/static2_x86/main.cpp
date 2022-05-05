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
//�ź�������
sem_t sem_leader;
sem_t sem_Division[NUM_THREADS-1];
sem_t sem_Eimination[NUM_THREADS-1];

typedef struct{
    int t_id;    //�߳�id
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

void *threadFunc(void *param)  //����ѭ������
{
    threadParam_t *p = (threadParam_t*)param;
    int t_id=p->t_id;   //�̱߳��
    for(int k=0;k<N;k++)
    {
        if(t_id==0)
        {
            for(int j=k+1;j<N;j++)    //t_idΪ0���߳�����������,���������߳��ȵȴ�
                m[k][j]=m[k][j]/m[k][k];
            m[k][k]=1.0;
        }
        else
            sem_wait(&sem_Division[t_id-1]);   // �������ȴ���ɳ�������
        if(t_id==0)               // t_id Ϊ0 ���̻߳������������̣߳�������ȥ����
            for(int i=0;i<NUM_THREADS-1;i++)
                sem_post(&sem_Division[i]);

        for(int i=k+1+t_id;i<N;i+=NUM_THREADS)
        {
            for(int j=k+1;j<N;j++)
                m[i][j]=m[i][j]-m[i][k]*m[k][j];
            m[i][k]=0;
        }
        if(t_id==0)
        {
            for(int i=0;i<NUM_THREADS-1;i++)   //�ȴ�����worker �����ȥ
                sem_wait(&sem_leader);
            for(int i=0;i<NUM_THREADS-1;i++)
                sem_post(&sem_Eimination[i]);   // ֪ͨ����worker ������һ��
        }
        else{
            sem_post(&sem_leader);   //֪ͨleader, �������ȥ����
            sem_wait(&sem_Eimination[t_id-1]); //�������ȴ�������һ��
        }
    }
    pthread_exit(NULL);
}
void *threadFunc_1(void *param)  //���н��п黮��
{
    threadParam_t *p = (threadParam_t*)param;
    int t_id=p->t_id;   //�̱߳��
    for(int k=0;k<N;k++)
    {
        if(t_id==0)
        {
            for(int j=k+1;j<N;j++)    //t_idΪ0���߳�����������,���������߳��ȵȴ�
                m[k][j]=m[k][j]/m[k][k];
            m[k][k]=1.0;
        }
        else
            sem_wait(&sem_Division[t_id-1]);   // �������ȴ���ɳ�������
        if(t_id==0)               // t_id Ϊ0 ���̻߳������������̣߳�������ȥ����
            for(int i=0;i<NUM_THREADS-1;i++)
                sem_post(&sem_Division[i]);

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

        if(t_id==0)
        {
            for(int i=0;i<NUM_THREADS-1;i++)   //�ȴ�����worker �����ȥ
                sem_wait(&sem_leader);
            for(int i=0;i<NUM_THREADS-1;i++)
                sem_post(&sem_Eimination[i]);   // ֪ͨ����worker ������һ��
        }
        else{
            sem_post(&sem_leader);   //֪ͨleader, �������ȥ����
            sem_wait(&sem_Eimination[t_id-1]); //�������ȴ�������һ��
        }
    }
    pthread_exit(NULL);
}
void *threadFunc_2(void *param)  //���н��п黮��
{
    threadParam_t *p = (threadParam_t*)param;
    int t_id=p->t_id;   //�̱߳��
    for(int k=0;k<N;k++)
    {
        if(t_id==0)
        {
            for(int j=k+1;j<N;j++)    //t_idΪ0���߳�����������,���������߳��ȵȴ�
                m[k][j]=m[k][j]/m[k][k];
            m[k][k]=1.0;
        }
        else
            sem_wait(&sem_Division[t_id-1]);   // �������ȴ���ɳ�������
        if(t_id==0)               // t_id Ϊ0 ���̻߳������������̣߳�������ȥ����
            for(int i=0;i<NUM_THREADS-1;i++)
                sem_post(&sem_Division[i]);

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

        if(t_id==0)
        {
            for(int i=0;i<NUM_THREADS-1;i++)   //�ȴ�����worker �����ȥ
                sem_wait(&sem_leader);
            for(int i=0;i<NUM_THREADS-1;i++)
                sem_post(&sem_Eimination[i]);   // ֪ͨ����worker ������һ��
        }
        else{
            sem_post(&sem_leader);   //֪ͨleader, �������ȥ����
            sem_wait(&sem_Eimination[t_id-1]); //�������ȴ�������һ��
        }
    }
    pthread_exit(NULL);
}
void *threadFunc_3(void *param)  //���н���ѭ������
{
    threadParam_t *p = (threadParam_t*)param;
    int t_id=p->t_id;   //�̱߳��
    for(int k=0;k<N;k++)
    {
        if(t_id==0)
        {
            for(int j=k+1;j<N;j++)    //t_idΪ0���߳�����������,���������߳��ȵȴ�
                m[k][j]=m[k][j]/m[k][k];
            m[k][k]=1.0;
        }
        else
            sem_wait(&sem_Division[t_id-1]);   // �������ȴ���ɳ�������
        if(t_id==0)               // t_id Ϊ0 ���̻߳������������̣߳�������ȥ����
            for(int i=0;i<NUM_THREADS-1;i++)
                sem_post(&sem_Division[i]);

        for(int i=k+1;i<N;i++)
        {
            for(int j=k+1+t_id;j<N;j+=NUM_THREADS)
                m[i][j]=m[i][j]-m[i][k]*m[k][j];
            m[i][k]=0;
        }

        if(t_id==0)
        {
            for(int i=0;i<NUM_THREADS-1;i++)   //�ȴ�����worker �����ȥ
                sem_wait(&sem_leader);
            for(int i=0;i<NUM_THREADS-1;i++)
                sem_post(&sem_Eimination[i]);   // ֪ͨ����worker ������һ��
        }
        else{
            sem_post(&sem_leader);   //֪ͨleader, �������ȥ����
            sem_wait(&sem_Eimination[t_id-1]); //�������ȴ�������һ��
        }
    }
    pthread_exit(NULL);
}
void *threadFunc_4(void *param)  //���н���ѭ�����֣����ö��뷽ʽ��SSE���
{
    threadParam_t *p = (threadParam_t*)param;
    int t_id=p->t_id;   //�̱߳��
    __m128 vaik,vakj,vaij,vx;
    int j;
    for(int k=0;k<N;k++)
    {
        if(t_id==0)
        {
            for(int j=k+1;j<N;j++)    //t_idΪ0���߳�����������,���������߳��ȵȴ�
                m[k][j]=m[k][j]/m[k][k];
            m[k][k]=1.0;
        }
        else
            sem_wait(&sem_Division[t_id-1]);   // �������ȴ���ɳ�������
        if(t_id==0)               // t_id Ϊ0 ���̻߳������������̣߳�������ȥ����
            for(int i=0;i<NUM_THREADS-1;i++)
                sem_post(&sem_Division[i]);

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

        if(t_id==0)
        {
            for(int i=0;i<NUM_THREADS-1;i++)   //�ȴ�����worker �����ȥ
                sem_wait(&sem_leader);
            for(int i=0;i<NUM_THREADS-1;i++)
                sem_post(&sem_Eimination[i]);   // ֪ͨ����worker ������һ��
        }
        else{
            sem_post(&sem_leader);   //֪ͨleader, �������ȥ����
            sem_wait(&sem_Eimination[t_id-1]); //�������ȴ�������һ��
        }
    }
    pthread_exit(NULL);
}
void *threadFunc_5(void *param)  //���н���ѭ�����֣����ö��뷽ʽ��AVX���
{
    threadParam_t *p = (threadParam_t*)param;
    int t_id=p->t_id;   //�̱߳��
    __m256 vaik,vakj,vaij,vx;
    int j;
    for(int k=0;k<N;k++)
    {
        if(t_id==0)
        {
            for(int j=k+1;j<N;j++)    //t_idΪ0���߳�����������,���������߳��ȵȴ�
                m[k][j]=m[k][j]/m[k][k];
            m[k][k]=1.0;
        }
        else
            sem_wait(&sem_Division[t_id-1]);   // �������ȴ���ɳ�������
        if(t_id==0)               // t_id Ϊ0 ���̻߳������������̣߳�������ȥ����
            for(int i=0;i<NUM_THREADS-1;i++)
                sem_post(&sem_Division[i]);

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

        if(t_id==0)
        {
            for(int i=0;i<NUM_THREADS-1;i++)   //�ȴ�����worker �����ȥ
                sem_wait(&sem_leader);
            for(int i=0;i<NUM_THREADS-1;i++)
                sem_post(&sem_Eimination[i]);   // ֪ͨ����worker ������һ��
        }
        else{
            sem_post(&sem_leader);   //֪ͨleader, �������ȥ����
            sem_wait(&sem_Eimination[t_id-1]); //�������ȴ�������һ��
        }
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
		sem_init(&sem_leader,0,0);//��ʼ���ź���
		for(j=0;j<NUM_THREADS-1;j++)
		{
            sem_init(&sem_Division[j],0,0);
            sem_init(&sem_Eimination[j],0,0);
		}
		//�����߳�
		pthread_t handles[NUM_THREADS];
        threadParam_t param[NUM_THREADS];
        for(id=0;id<NUM_THREADS;id++)
        {
            param[id].t_id=id;
            pthread_create(&handles[id], NULL, threadFunc, (void *)&param[id]);    //���н���ѭ������
            //pthread_create(&handles[id], NULL, threadFunc_1, (void *)&param[id]);  //���н��п黮��
            //pthread_create(&handles[id], NULL, threadFunc_2, (void *)&param[id]);  //���н��п黮��
            //pthread_create(&handles[id], NULL, threadFunc_3, (void *)&param[id]);  //���н���ѭ������
            //pthread_create(&handles[id], NULL, threadFunc_4, (void *)&param[id]);  //���н���ѭ�����֣����ö��뷽ʽ��SSE���
            //pthread_create(&handles[id], NULL, threadFunc_5, (void *)&param[id]);    //���н���ѭ�����֣����ö��뷽ʽ��AVX���

        }
        for(id=0;id<NUM_THREADS;id++)
            pthread_join(handles[id], NULL);
        sem_destroy(&sem_leader);//���������ź���
		for(j=0;j<NUM_THREADS-1;j++)
		{
            sem_destroy(&sem_Division[j]);
            sem_destroy(&sem_Eimination[j]);
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
