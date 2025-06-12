#include<omp.h>
#include<iostream>
using namespace std;

int main(){

    int arr[100];
    int f=2;
    int last_v=0;
for (int i = 0; i < 100; i++) 
arr[i] = i;


#pragma omp parallel for shared(arr) firstprivate(f) lastprivate(last_v)
{
for (int i = 0; i < 100; i++)
{
   arr[i]*=f;
   last_v=arr[i];
}
}
int sum=0;
#pragma omp parallel for reduction(+:sum) shared(arr)
{
    for(int i=0;i<100;i++)
    sum+=arr[i];
}

printf("sum= %d\n ",sum);
printf("last value is %d\n",last_v);
cout<<"num of threads"<<omp_get_num_threads()<<endl;
}