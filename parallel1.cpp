#include<iostream>
#include<pthread.h>
using namespace std;


void* fun(void* arg) {
cout << "Thread ID from threadFunction: " << pthread_self() <<endl;
  return NULL;
}
int main() {

  const int numThreads = 5;
  pthread_t threads[numThreads];
  for (int i = 0; i < numThreads; ++i) {
      pthread_create(&threads[i], NULL, fun, NULL);
  }

 for (int i = 0; i < numThreads; ++i) {
      pthread_join(threads[i], NULL);
  }
  
  printf("Main thread exiting.\n");
  cout << "Main thread ID " << pthread_self() <<endl;

  return 0;
}
