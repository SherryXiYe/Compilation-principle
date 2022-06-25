#include <iostream>
#include<stdlib.h>
#include <stdio.h>
#include<math.h>
#include <time.h>
#include <semaphore.h>
#include<omp.h>
#include <immintrin.h>
#include<malloc.h>
#include <stdint.h>
using namespace std;
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

void avx_unaligned_t()  //unaligned. Only the triple loop is SIMD programmed.
{
	int i,j,k;
	__m256 vaik,vakj,vaij,vx;
    for(k=0;k<N;k++)
    {
        for(j=k+1;j<N;j++)
            m[k][j]=m[k][j]/m[k][k];
        m[k][k]=1.0;
		for(i=k+1;i<N;i++)
        {
            vaik=_mm256_set_ps(m[i][k],m[i][k],m[i][k],m[i][k],m[i][k],m[i][k],m[i][k],m[i][k]);
            for(j=k+1;j+8<=N;j+=8)
            {
                vakj=_mm256_loadu_ps(m[k]+j);
                vaij=_mm256_loadu_ps(m[i]+j);
                vx=_mm256_mul_ps(vakj,vaik);
                vaij=_mm256_sub_ps(vaij,vx);
                _mm256_storeu_ps(m[i]+j,vaij);
            }
            for(j=j-8;j<N;j++)
            //Calculates several elements at the end of the line
                m[i][j]=m[i][j]-m[i][k]*m[k][j];
            m[i][k]=0;
        }
    }

}

void avx_aligned_t()  //aligned. Only the triple loop is SIMD programmed.
{
        int i,j,k;
        __m256 vaik,vakj,vaij,vx;
        for(k=0;k<N;k++)
        {
            for(j=k+1;j<N;j++)
                m[k][j]=m[k][j]/m[k][k];
            m[k][k]=1.0;
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
        }

}
void m_out()
{
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
			cout << m[i][j] << " ";
		cout << endl;
	}

}

int main()
{
	struct timespec sts, ets;
	int x;
	int n[10]={10,50,100,200,300,500,1000,2000,3000,4000};
	for(x=0;x<1;x++)
	{
		N=n[x];
		m = new float* [N];
		for (int i = 0; i < N; i++)
			m[i] = new float[N];
		for (int i = 0; i < N; i++)
			for (int j = 0; j < N; j++)
				m[i][j] = (i + 1) * (j + 1) + 1;
        
	seq();
	m_out();
                for (int i = 0; i < N; i++)
                        for (int j = 0; j < N; j++)
                                m[i][j] = (i + 1) * (j + 1) + 1;	
	timespec_get(&sts, TIME_UTC);
	//to measure
	avx_unaligned_t();
	//avx_aligned_t();
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
	cout<<"avx"<<endl;
	m_out();
	for(int j=0;j<N;j++)
            delete[] m[j];
        delete[]m;
	}
	return 0;
}
