#include<iostream>
#include<pthread.h>
#include<unistd.h>
using namespace std;

struct threadData {
int id;
int va;
};

int arr[5];

void* fun(void* arg){
    struct threadData* data =(struct threadData*)arg;
    cout<<data ->id<<arr[data->id]<<endl;
    //return threadData structure instead of thread id
    return NULL;
}//fun

int main(){

const int numThreads =5;
pthread_t threads[numThreads];

threadData data[numThreads];

for(int i=0;i<numThreads;i++){
    arr[i]=i;
}

//create threads
for(int i=0;i<numThreads;i++){
    
    data[i].id=i;
    pthread_create(&threads[i],NULL,fun,&data[i]);

}

//join threads and retrieve their return values 
for(int i=0;i<numThreads;i++){
    pthread_join(threads[i],NULL);//correctly cast to retrieve thread id
}

return 0;

}//main