#include<stdio.h>
int a=0;
int isPrime(int n){
    if(n==1)
        return 0;
    int i=2;
    for(i=2;i<n;i++){
        if(n%i==0)
            return 0;
    }
    return 1;
}
int main(){
    scanf("%d", &a);
    if(isPrime(a)){
        printf("Prime number\n");
    }else
        printf("Composite number\n");
    return 0;
}