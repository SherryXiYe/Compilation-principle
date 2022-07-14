#include <iostream>
#include <fstream>
#include <cassert>
#include <string>
#include<sstream>
#include <vector>

#include <time.h>
#include<math.h>
#include<stdlib.h>
#include <semaphore.h>
#include <immintrin.h>
#include<malloc.h>
#include <stdio.h>
#include <stdint.h>

#include<omp.h>

using namespace std;

const int Col[11]={130,254,562,1011,2362,3799,8399,23045,37960,43577,85401};          //矩阵列数
const int Eliminator[11]={22,106,170,539,1226,2759,6375,18748,29304,39477,5724};     //非零消元子行数
const int Eliminated[11]={8,53,53,263,453,1953,4535,14325,14921,54274,756};      //被消元行数

const string ed_Path[11]={
    "/home/u153282/workplace/Groebner_data/1/被消元行.txt",
    "/home/u153282/workplace/Groebner_data/2/被消元行.txt",
    "/home/u153282/workplace/Groebner_data/3/被消元行.txt",
    "/home/u153282/workplace/Groebner_data/4/被消元行.txt",
    "/home/u153282/workplace/Groebner_data/5/被消元行.txt",
    "/home/u153282/workplace/Groebner_data/6/被消元行.txt",
    "/home/u153282/workplace/Groebner_data/7/被消元行.txt",
    "/home/u153282/workplace/Groebner_data/8/被消元行.txt",
    "/home/u153282/workplace/Groebner_data/9/被消元行.txt",
    "/home/u153282/workplace/Groebner_data/10/被消元行.txt",
    "/home/u153282/workplace/Groebner_data/11/被消元行.txt",
};
const string er_Path[11]={
    "/home/u153282/workplace/Groebner_data/1/消元子.txt",
    "/home/u153282/workplace/Groebner_data/2/消元子.txt",
    "/home/u153282/workplace/Groebner_data/3/消元子.txt",
    "/home/u153282/workplace/Groebner_data/4/消元子.txt",
    "/home/u153282/workplace/Groebner_data/5/消元子.txt",
    "/home/u153282/workplace/Groebner_data/6/消元子.txt",
    "/home/u153282/workplace/Groebner_data/7/消元子.txt",
    "/home/u153282/workplace/Groebner_data/8/消元子.txt",
    "/home/u153282/workplace/Groebner_data/9/消元子.txt",
    "/home/u153282/workplace/Groebner_data/10/消元子.txt",
    "/home/u153282/workplace/Groebner_data/11/消元子.txt",
};

int col;                    //矩阵列数
int eliminator;     //非零消元子行数
int eliminated;      //被消元行数
string ed_path;
string er_path;

int** ed;
int** er;

int col_e;
int now_index=-1;

void txt_to_vectorint(vector<vector<int>>& res, string pathname)
{
	ifstream infile;
	infile.open(pathname.data());   //将文件流对象与文件连接起来
	assert(infile.is_open());   //若失败,则输出错误消息,并终止程序运行
	vector<int> suanz;
	string s;
	while (getline(infile, s)) {
		istringstream is(s);   //将读出的一行转成数据流进行操作
		int f;
		while(is>>f){
			suanz.push_back(f);
		}
		res.push_back(suanz);
		suanz.clear();
		s.clear();
	}
	infile.close();             //关闭文件输入流
}

void list_to_bit_ed(vector<vector<int>>& data)
{
    col_e=col/31;
    if(col%31!=0)
        col_e++;
    //cout<<"2.1"<<endl;
    int i,j;
    /*
    //avx
    ed=(int **)aligned_alloc(1024,eliminated* sizeof(int*));
	for(i=0;i<eliminated;i++)
        ed[i]=(int *)aligned_alloc(1024, col_e* sizeof(int));
    */
    ed=new int*[eliminated];
    for(i=0;i<eliminated;i++)
        ed[i]=new int[col_e];
    
    for(i=0;i<eliminated;i++)
        for(j=0;j<col_e;j++)
            ed[i][j]=0;
    //cout<<"2.2"<<endl;
    for(i=0;i<eliminated;i++)
    {
        int si=data[i].size();
        
        //cout<<i+1<<"行size: "<<si<<endl;
        for(j=0;j<si;j++)
        {
            int r=data[i][j]%31;
            int p=data[i][j]/31;
            ed[i][p]+=(int)(1<<(30-r));
        }
        //cout<<"end"<<endl;
    }
    //cout<<"2.3"<<endl;
}

void list_to_bit_er(vector<vector<int>>& data)
{
    int i,j;
    /*
    //avx
    er=(int **)aligned_alloc(1024,col* sizeof(int*));
	for(i=0;i<col;i++)
        er[i]=(int *)aligned_alloc(1024, col_e* sizeof(int));
    */
    er=new int*[col];
    for(i=0;i<col;i++)
        er[i]=new int[col_e];
    
    for(i=0;i<col;i++)
        for(j=0;j<col_e;j++)
            er[i][j]=0;
    int d=eliminator-1;
    for(i=0;i<col;i++)
    {
        if(data[d][0]==i&&d>=0)
        {
            int si=data[d].size();
            for(j=0;j<si;j++)
            {
                int r=data[d][j]%31;
                int p=data[d][j]/31;
                er[i][p]+=(int)(1<<(30-r));
            }
            d--;
        }
    }
}

void bit_to_list(vector<vector<int>>& res)
{
    int i,j;
    vector<int> t;
    for(i=0;i<eliminated;i++)
    {
        for(j=col_e-1;j>=0;j--)
        {
            unsigned int e=ed[i][j];
            for(int ie=0;ie<31;ie++)
            {
                if((e>>ie)&1)
                {
                    int num=j*31+30-ie;
                    t.push_back(num);
                }
            }
        }
        res.push_back(t);
        t.clear();
    }
}

void gauss_seq()
{
    int i,j,k,index;
    bool finish=false;
    //#pragma omp parallel num_threads(NUM_THREADS), private(i, j, k, index,finish)
    for(i=0;i<eliminated;i++)
    {
        finish=false;
        for(j=col_e-1;j>=0&&finish==false;j--)
        {
            if(ed[i][j]!=0)
            {
                int r;
                for(r=0;r<31;r++)
                {
                    if((ed[i][j]>>r)&1)
                        index=j*31+30-r;
                    else
                        continue;
                    bool exist=false;
                    for(k=0;k<col_e;k++)
                        if(er[index][k]!=0)
                        {
                            exist=true;
                            break;
                        }
                    if(exist)
                    {
                        // 并行部分，使用行划分
                        //int c=ceil((j+1)/(double)NUM_THREADS);
                        //#pragma omp for schedule(static,c)
                        //#pragma omp parallel for num_threads(NUM_THREADS), private(j, k)//,schedule(static,30)
                        for(k=j;k>=0;k--)
                            ed[i][k]=ed[i][k]^er[index][k];
                    }else{
                        // 并行部分，使用行划分
                        //int c=ceil((j+1)/(double)NUM_THREADS);
                        //#pragma omp for schedule(static,c)
                        for(k=j;k>=0;k--)
                            er[index][k]=ed[i][k];
                        finish=true;
                        break;
                    }
                }
            }
        }
    }
}

void gauss_seq2()
{
    int i,j,k,r,index,now_index;
    bool finish=false;
    int finished_count=0;
    for(;finished_count<eliminated;finished_count++)
    {
        now_index=-1;
        for(i=finished_count;i<eliminated;i++)
        {
            finish=false;
            for(j=col_e-1;j>=0&&finish==false;j--)
            {
                if(ed[i][j]!=0)
                {
                    for(r=0;r<31;r++)
                    {
                        if((ed[i][j]>>r)&1)
                            index=j*31+30-r;
                        else
                            continue;
                        bool exist=false;
                        for(k=0;k<col_e;k++)
                            if(er[index][k]!=0)
                            {
                                exist=true;
                                break;
                            }
                        if(exist)
                        {
                            /*
                            for(k=j;k>=0;k--)
                                ed[i][k]=ed[i][k]^er[index][k];
                            */
                             
                            //sse_u
                            __m128i vedik,verik;
                            for(k=j;k-4>=-1;k-=4)
                            {
                                vedik=_mm_loadu_epi32((__m128i*)(ed[i]+k-3));
                                verik=_mm_loadu_epi32((__m128i*)(er[index]+k-3));
                                vedik=_mm_xor_epi32(vedik,verik);
                                _mm_storeu_epi32((__m128i*)(ed[i]+k-3),vedik);
                            }
                            for(;k>=0;k--)
                                ed[i][k]=ed[i][k]^er[index][k];
                            
                            /*
                            //avx_u
                            __m256i vedik,verik;
                            for(k=j;(k%8!=0)&&k>=0;k--)
                            ed[i][k]=ed[i][k]^er[index][k];
                            for(;k-8>=-1;k-=8)
                            {
                                vedik=_mm256_load_epi32(ed[i]+k-7);
                                verik=_mm256_load_epi32(er[index]+k-7);
                                vedik=_mm256_xor_epi32(vedik,verik);
                                _mm256_store_epi32(ed[i]+k-7,vedik);
                            }
                            for(;k>=0;k--)
                                ed[i][k]=ed[i][k]^er[index][k];
                            */
                            /* 
                            //avx512_u
                            __m512i vedik,verik;
                            for(k=j;k-16>=-1;k-=16)
                            {
                                vedik=_mm512_loadu_epi32(ed[i]+k-15);
                                verik=_mm512_loadu_epi32(er[index]+k-15);
                                vedik=_mm512_xor_epi32(vedik,verik);
                                _mm512_storeu_epi32(ed[i]+k-15,vedik);
                            }
                            for(;k>=0;k--)
                                ed[i][k]=ed[i][k]^er[index][k];
                            */
                        }else{
                            if(i==finished_count)
                                now_index=index;
                            finish=true;
                            break;
                        }
                    }
                }
            }
        }
        if(now_index!=-1)
            for(k=0;k<col_e;k++)
                er[now_index][k]=ed[finished_count][k];
    }
}

void out_ed()
{
    int i,j;
    cout<<"ed"<<endl;
     for(i=0;i<eliminated;i++)
     {
        for(j=0;j<col_e;j++)
            cout<<ed[i][j]<<" ";
        cout<<endl;
     }
    for(i=0;i<eliminated;i++)
    {
        for(j=0;j<col_e;j++)
        {
            unsigned int e=ed[i][j];
            for(int ie=30;ie>=0;ie--)
                cout<<((e>>ie)&1);
            cout<<" ";
        }
        cout<<endl;
    }
}

void out_er()
{
    int i,j;
    cout<<"er:"<<endl;
     for(i=0;i<col;i++)
     {
        for(j=0;j<col_e;j++)
            cout<<er[i][j]<<" ";
        cout<<endl;
     }
    for(i=0;i<col;i++)
    {
        for(j=0;j<col_e;j++)
        {
            unsigned int e=er[i][j];
            for(int ie=30;ie>=0;ie--)
                cout<<((e>>ie)&1);
            cout<<" ";
        }
        cout<<endl;
    }
}

int NUM_THREADS=5;

int main()
{
    struct timespec sts, ets;

    vector<vector<int>> er_data,ed_data;
    int x=0;
    col=Col[x];                    //矩阵列数
    eliminator=Eliminator[x];     //非零消元子行数
    eliminated=Eliminated[x];      //被消元行数
    ed_path=ed_Path[x];
    er_path=er_Path[x];

    //cout<<"1"<<endl;
    txt_to_vectorint(er_data,er_path);
    txt_to_vectorint(ed_data,ed_path);
    //cout<<"2"<<endl;
    list_to_bit_ed(ed_data);
    //cout<<"3"<<endl;
    list_to_bit_er(er_data);
    //cout<<"4"<<endl;
    //out_er();
    //out_ed();

    timespec_get(&sts, TIME_UTC);
	//to measure
    
    int i,j,k,r,index;
    bool finish=false;
    int finished_count=0;
    #pragma omp parallel num_threads(NUM_THREADS), private(i, j, k, index,finish,finished_count)//,vedik,verik)
    for(;finished_count<eliminated;finished_count++)
    {
        now_index=-1;
        #pragma omp for schedule(guided,1)
        for(i=finished_count;i<eliminated;i++)
        {
            finish=false;
            for(j=col_e-1;j>=0&&finish==false;j--)
            {
                if(ed[i][j]!=0)
                {
                    for(r=0;r<31;r++)
                    {
                        if((ed[i][j]>>r)&1)
                            index=j*31+30-r;
                        else
                            continue;
                        bool exist=false;
                        for(k=0;k<col_e;k++)
                            if(er[index][k]!=0)
                            {
                                exist=true;
                                break;
                            }
                        if(exist)
                        {
                            
                            for(k=j;k>=0;k--)
                                ed[i][k]=ed[i][k]^er[index][k];
                            /*
                            //sse_u
                            __m128i vedik,verik;
                            for(k=j;k-4>=-1;k-=4)
                            {
                                vedik=_mm_loadu_epi32((__m128i*)(ed[i]+k-3));
                                verik=_mm_loadu_epi32((__m128i*)(er[index]+k-3));
                                vedik=_mm_xor_epi32(vedik,verik);
                                _mm_storeu_epi32((__m128i*)(ed[i]+k-3),vedik);
                            }
                            for(;k>=0;k--)
                                ed[i][k]=ed[i][k]^er[index][k];
                            */
                            /*
                            //avx_u
                            //__m256i vedik,verik;
                            for(k=j;k-8>=-1;k-=8)
                            {
                                vedik=_mm256_loadu_epi32(ed[i]+k-7);
                                verik=_mm256_loadu_epi32(er[index]+k-7);
                                vedik=_mm256_xor_epi32(vedik,verik);
                                _mm256_storeu_epi32((ed[i]+k-7),vedik);
                            }
                            for(;k>=0;k--)
                                ed[i][k]=ed[i][k]^er[index][k];
                            */    
                            /*
                            //avx512_u
                            __m512i vedik,verik;
                            for(k=j;k-16>=-1;k-=16)
                            {
                                vedik=_mm512_loadu_epi32(ed[i]+k-15);
                                verik=_mm512_loadu_epi32(er[index]+k-15);
                                vedik=_mm512_xor_epi32(vedik,verik);
                                _mm512_storeu_epi32(ed[i]+k-15,vedik);
                            }
                            for(;k>=0;k--)
                                ed[i][k]=ed[i][k]^er[index][k];
                            */
                        }else{
                            if(i==finished_count)
                                now_index=index;
                            finish=true;
                            break;
                        }
                    }
                }
            }
        }
        #pragma omp single
        {
            if(now_index!=-1)
                for(k=0;k<col_e;k++)
                    er[now_index][k]=ed[finished_count][k];
        }
    }
      
    //gauss_seq();
    //gauss_seq2();
    
    // end time
    timespec_get(&ets, TIME_UTC);
    //cout<<"5"<<endl;
    time_t dsec = ets.tv_sec - sts.tv_sec;
    long dnsec = ets.tv_nsec - sts.tv_nsec;
    if (dnsec < 0)
    {
        dsec--;
        dnsec += 1000000000ll;
    }
    printf("%lld.%09lld\n", dsec, dnsec);
    /*
    vector<vector<int>> result;
    bit_to_list(result);
    for(int i=0;i<eliminated;i++)
    {
        int si=result[i].size();
        for(int j=0;j<si;j++)
            cout<<result[i][j]<<" ";
        cout<<endl;
    }
    /*
    //avx
    for(i=0;i<eliminated;i++)
        free(ed[i]);
    free(ed);
    for(i=0;i<col;i++)
        free(er[i]);
    free(er);
    */
    for(int i=0;i<eliminated;i++)
        delete[] ed[i];
    delete[] ed;
    for(int i=0;i<col;i++)
        delete[] er[i];
    delete[] er;
    
    return 0;
}

