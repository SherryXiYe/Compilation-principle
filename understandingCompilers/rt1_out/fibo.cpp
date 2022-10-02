#include<iostream>
using namespace std;
#define  OUT cout
#define MYADD(a,b) (a+b)
int globe_n;
int myplus(int x,int y)    
{
    return x+y;
}
int main()            //斐波拉契数列
{
    int a, b, i, t;
    a = 0; 
    b = 1;
    i = 1;
    const int add=1;
    cin >> globe_n;
    OUT << a << endl;
    OUT << b << endl;
    while (i < globe_n)
    {
        t = b;
        b = myplus(a,b);
        OUT << b << endl;     //cout<<b<<endl
        a = t;
        i = MYADD(i,add);     //i=i+add
    }
    return 0;
}