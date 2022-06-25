#include<iostream>
#include<stdlib.h>
#include <stdio.h>
#include<math.h>
#include <time.h>
#include <semaphore.h>
#include<omp.h>
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
	int i, j, k;
	for (k = 0; k < N; k++)
	{
		for (j = k + 1; j < N; j++)
			m[k][j] = m[k][j] / m[k][k];
		m[k][k] = 1.0;
		for (i = k + 1; i < N; i++)
		{
			for (j = k + 1; j < N; j++)
				m[i][j] = m[i][j] - m[i][k] * m[k][j];
			m[i][k] = 0;
		}
	}
}

int main(int argc, char* argv[])
{
	struct timespec sts, ets;
	int n[10] = { 10,50,100,200,300,500,1000,2000,3000,4000 };
	N = n[0];
	//m_reset();
	
	int myid, size;
	int my_n;
	int first, last;
	m = new float* [N];
	for (int i = 0; i < N; i++)
		m[i] = new float[N];
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			m[i][j] = (i + 1) * (j + 1) + 1;
	
	MPI_Status status;
	timespec_get(&sts, TIME_UTC);
        //to measure

	MPI_Init(&argc, &argv);
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
	int i,j,k;
	for(k=0;k<N;k++)
	{
		if(first<=k&&k<=last)
		{
			for ( j = k + 1; j < N; j++)
                                m[k][j] = m[k][j] / m[k][k];
                        m[k][k] = 1.0;
			if(myid!=size-1){
                                MPI_Send(&m[k][0],N,MPI_FLOAT,myid+1,k+1,MPI_COMM_WORLD);
                        }
			for (i=k+1;i <= last; i++)
			{
                                for (j = k + 1; j < N; j++)
                                        m[i][j] = m[i][j] - m[i][k] * m[k][j];
                                m[i][k] = 0;
			}
                }else if(k<first)
		{
                        MPI_Recv(&m[k][0],N,MPI_FLOAT,myid-1,k+1,MPI_COMM_WORLD,&status);
			if(myid!=size-1)
				MPI_Send(&m[k][0],N,MPI_FLOAT,myid+1,k+1,MPI_COMM_WORLD);
			for(i=first;i<=last;i++)
			{
				for (j = k + 1; j < N; j++)
                                        m[i][j] = m[i][j] - m[i][k] * m[k][j];
                                m[i][k] = 0;
			}
		}
	}
	if(myid!=0)
		MPI_Send(&m[first][0],(last-first+1)*N,MPI_FLOAT,0,myid,MPI_COMM_WORLD);
	else{
		for(int id=1;id<size;id++)
		{
			int f,l;
			if (id < N % size)
        		{
                		f = id * (my_n + 1);
                		l = f + my_n;
        		}
        		else
        		{
                		f = N % size * (my_n + 1) + (id - N % size) * my_n;
                		l = f + my_n - 1;
        		}
			MPI_Recv(&m[f][0],N*(l-f+1),MPI_FLOAT,id,id,MPI_COMM_WORLD,&status);
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
	return 0;
}
