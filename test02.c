#include <stdio.h> 

int main(){
    int arr1[6]={1,2,3,4,5,6};
    int arr2[2][3]={{1,2,3},{4,5,6}};
    int* p1 = arr1;
    int* p2 = arr2;
    int* p3 = arr2[1];
   

    printf("data1=%d : %d \n",arr1[3],*(arr1+3));
    printf("data2=%d : %d \n",arr2[1][1],*(p3+1));

    return 1;
}   