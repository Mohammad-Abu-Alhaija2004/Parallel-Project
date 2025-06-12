#include<iostream>
#include<pthread.h>
using namespace std;

void* fun1(void* d){
    int *d_v =(int*) d;
    cout<<"Thread ID:"<<*d_v<<endl;

    return NULL;
 }

 int main(){
  pthread_t ths[10];
  int index[10];
  
  for(int i=0;i<10;i++){
    index[i]=i;
    pthread_create(&ths[i],NULL,fun1,(void *) &index[i]);
  }

  for(int i=0;i<10;i++){
    pthread_join(ths[i],NULL);
  }

  return 0;

 }