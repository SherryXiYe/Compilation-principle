#include <stdio.h>
int main(){
    int a[10],i;
    for(i=0;i<10;i++){
        scanf("%d",&a[i]);
    }
    i=0;
    int j=9;
    while (i<j)
    {   
        int t=a[i];
        a[i]=a[j];
        a[j]=t;
        i++;
        j--;
    }
    printf("Array after rotation:\n");
    for(i=0;i<10;i++){
        printf("%d  ",a[i]);
    }
    printf("\n");
    return 0;
}