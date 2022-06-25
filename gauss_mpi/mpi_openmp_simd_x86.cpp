#include<iostream>
#include<stdlib.h>
#include <stdio.h>
#include<math.h>
#include <time.h>
#include <semaphore.h>
#include<omp.h>
#include <immintrin.h>
#include<malloc.h>
#include <stdint.h>
#include"mpi.h"
using namespace std;

int N = 0;
float** m;


void m_reset()
{
	m = new float* [N];
                for (int i = 0; i < N; i++)
                        m[i] = new float[N];
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < i; j++)
			m[i][j] = 0;
		m[i][i] = 1.0;
		for (int j = i + 1; j < N; j++)
			m[i][j] = rand();
	}
	for (int k = 0; k < N; k++)
		for (int i = k + 1; i < N; i++)
			for (int j = 0; j < N; j++)
				m[i][j] += m[k][j];
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

int max(int a,int b){
    return a>b?a:b;
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

int NUM_THREADS = 4;

int main(int argc, char* argv[])
{
	struct timespec sts, ets;
	int n[10] = { 10,50,100,200,300,500,1000,2000,3000,4000 };
	N = n[9];
	m_reset();
	//m_reset_avx();  //avx

	int myid, size;
	int my_n;
	int first, last;
	int provided;
	
	/*m = new float* [N];
	for (int i = 0; i < N; i++)
		m[i] = new float[N];
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			m[i][j] = (i + 1) * (j + 1) + 1;
	*/
	MPI_Status status;
	timespec_get(&sts, TIME_UTC);
        //to measure

	//MPI_Init(&argc,&argv);
	MPI_Init_thread(&argc,&argv,MPI_THREAD_FUNNELED,&provided);
	if(provided<MPI_THREAD_FUNNELED)
	{
		MPI_Abort(MPI_COMM_WORLD,1);
    	}

	MPI_Comm_rank(MPI_COMM_WORLD, &myid);
	int i,j,k;
	//__m256 vaik,vakj,vaij,vx;  //avx
        //__m128 vaik,vakj,vaij,vx;  //sse
	#pragma omp parallel num_threads(NUM_THREADS), private(i, j, k, myid,first,last,size,my_n) //,vaik,vakj,vaij,vx)
	for(k=0;k<N;k++)
	{
		MPI_Comm_rank(MPI_COMM_WORLD, &myid);
        	MPI_Comm_size(MPI_COMM_WORLD, &size);
		my_n = N / size;
	 	if (myid < N % size)
        	{
                	first = myid * (my_n + 1);
                	last = first + my_n;
        	}
        	else
        	{
                	first = N % size * (my_n + 1) + (myid - N % size) * my_n;
                	last = first + my_n - 1;
        	}
		
		if(myid==0)
		{
			 #pragma omp single
			{
				for ( j = k + 1; j < N; j++)
                                	m[k][j] = m[k][j] / m[k][k];
                        	m[k][k] = 1.0;
				for(int id=1;id<size;id++){
                        	        MPI_Send(&m[k][0],N,MPI_FLOAT,id,k+1,MPI_COMM_WORLD);
                	        }
			}
                }else{
			#pragma omp single
			{
                        	MPI_Recv(&m[k][0],N,MPI_FLOAT,0,k+1,MPI_COMM_WORLD,&status);
			}
		}
		if (k+1 <= last)
                {
			#pragma omp for schedule(guided,1) 
                        for ( i=max(first,k+1); i <= last; i++)
                        {
				
				for (j = k + 1; j < N; j++)
                                	m[i][j] = m[i][j] - m[i][k] * m[k][j];
				/*
				//avx
				vaik=_mm256_set_ps(m[i][k],m[i][k],m[i][k],m[i][k],m[i][k],m[i][k],m[i][k],m[i][k]);  //avx
				for(j=k+1;(j%8!=0)&&j<N;j++)  //avx
                                	m[i][j] = m[i][j] - m[i][k] * m[k][j];
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
				*/
				/*
				//sse
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
				*/
				m[i][k] = 0;
				if(i==k+1&&myid!=0)
					MPI_Send(&m[i][0], N, MPI_FLOAT, 0, 0, MPI_COMM_WORLD);
                        }

                }
		if(myid==0&&last<k+1&&k+1<N)
		{
			#pragma omp single
                	{
				MPI_Recv(&m[k + 1][0], N, MPI_FLOAT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
			}
		}
	}
	MPI_Finalize();		
	// end time
	if(myid==0)
	{
		timespec_get(&ets, TIME_UTC);
		time_t dsec = (time_t)(ets.tv_sec - sts.tv_sec);
		long dnsec = ets.tv_nsec - sts.tv_nsec;
		if (dnsec < 0)
		{
			dsec--;
			dnsec += 1000000000ll;
		}
		cout << "time: "<<endl;
		printf("%ld.%09ld\n", dsec, dnsec);
		/*m_out();
        	for (int i = 0; i < N; i++)
                	for (int j = 0; j < N; j++)
                        	m[i][j] = (i + 1) * (j + 1) + 1;
		seq();
		m_out();*/
	}		
	for (int i = 0; i < N; i++) {
		delete[]m[i];
	}
	delete[]m;
	/*for(int j=0;j<N;j++)        //avx
        free(m[j]);
        free(m);*/
	return 0;
}
