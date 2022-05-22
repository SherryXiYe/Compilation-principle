#include <iostream>
#include <nmmintrin.h>
#include<math.h>
#include<stdlib.h>
#include <semaphore.h>
#include <immintrin.h>
#include<malloc.h>
#include <stdio.h>
#include <stdint.h>
#include<omp.h>
#include <time.h>
using namespace std;

int	NUM_THREADS=5;
int N=0;
float** m;

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
	m=(float **)aligned_alloc(1024,N* sizeof(float*));
	for(int i=0;i<N;i++)
        m[i]=(float *)aligned_alloc(1024, N* sizeof(float));
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

int main()
{
	struct timespec sts, ets;
	int x;
	int n[10]={10,50,100,200,300,500,1000,2000,3000,4000};
	for(x=0;x<10;x++)
	{
		N=n[x];
		//m_reset_avx();
		m_reset();
        timespec_get(&sts, TIME_UTC);
		//to measure
		int i,j,k;
		double tmp;
        //__m256 vaik,vakj,vaij,vx;  //avx
        //__m128 vaik,vakj,vaij,vx;  //sse
		#pragma omp parallel num_threads(NUM_THREADS), private(i, j, k, tmp  )//,vaik,vakj,vaij,vx)
        for(k=0;k<N;k++)
        {
            // 串行部分，也可以尝试并行化
            #pragma omp single
            {
                tmp = m[k][k];
                for(j=k+1;j<N;j++)
                    m[k][j]=m[k][j]/tmp;
                m[k][k]=1.0;
            }
            // 并行部分，使用行划分
           // int c=ceil((N-k-1)/(double)NUM_THREADS);
           #pragma omp for schedule(guided,1)
           // #pragma omp parallel for num_threads(NUM_THREADS), private(i, j, tmp),schedule(static,30)//,vaik,vakj,vaij,vx)
            for(i=k+1;i<N;i++)
            {
                tmp = m[i][k];
                for(j=k+1;j<N;j++)
                    m[i][j]=m[i][j]-tmp*m[k][j];
                m[i][k]=0;
            }
            /*
            //avx行块循环划分
             #pragma omp for schedule(dynamic, 30)
            for(i=k+1;i<N;i++)
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
            */
            /*
            //sse行循环划分
             #pragma omp for schedule(dynamic, 30)
            for(i=k+1;i<N;i++)
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
            */
            
            // 并行部分，使用列划分
            /*
            int c=ceil((N-k-1)/(double)NUM_THREADS);
            for(i=k+1;i<N;i++) 
            {
                tmp=m[i][k];
                #pragma omp for schedule(dynamic,c)
                 for(j=k+1;j<N;j++)
                    m[i][j]=m[i][j]-tmp*m[k][j];
                m[i][k]=0;
            }
            */
            // 离开for循环时，各个线程默认同步，进入下一行的处理
        }
		// end time
        timespec_get(&ets, TIME_UTC);
        time_t dsec = ets.tv_sec - sts.tv_sec;
        long dnsec = ets.tv_nsec - sts.tv_nsec;
        if (dnsec < 0)
        {
            dsec--;
            dnsec += 1000000000ll;
        }
        printf("%lld.%09lld\n", dsec, dnsec);
		for(int j=0;j<N;j++)
            delete[] m[j];
        delete[]m;
        //for(int j=0;j<N;j++)        //avx
          //  free(m[j]);
        //free(m);
	}
	return 0;
}

