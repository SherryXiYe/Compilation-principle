#include <iostream>
#include <windows.h>
#include<stdlib.h>
#pragma GCC optimize(3)
using namespace std;
int N=0;
float* a;
void a_reset()
{
    a=new float[N];
    for(int i=0;i<N;i++)
    {
        a[i]=i*i+50.0;
    }
}
int main()
{
    long long head, tail , freq ;
    int change=0,j,i;
    int n[10]={10,50,100,200,300,500,1000,2000,3000,4000};
    for(j=0;j<10;j++)
    {
        N=n[j];
        a_reset();
        QueryPerformanceFrequency((LARGE_INTEGER*)&freq ) ;
        // start time
        QueryPerformanceCounter((LARGE_INTEGER* )&head) ;
        //to measure
        /* LOOP #1*/
        for(i=0;i<N;i++)
        {
            a[i]=a[i]*2000;
            a[i]=a[i]/10000;
        }
        /* LOOP #2
        float* b=a;
        for(i=0;i<N;i++)
        {
            *b=*b*2000;
            *b=*b/10000;
            b++;
        }
        */
        // end time
        QueryPerformanceCounter((LARGE_INTEGER *)&tail ) ;
        cout << (double)(tail-head) * 1000.0 / (double)freq << endl;
        delete[]a;
    }
    return 0;
}

