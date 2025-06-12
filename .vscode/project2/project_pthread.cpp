#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <iomanip>
#include <random>
#include <pthread.h>

using namespace std;
using namespace std::chrono;

const int NUM_THREADS = 4;

struct ThreadData {
    vector<int>* arr;
    int exp;
    int start, end;
    int count[10];
};

pthread_barrier_t barrier; 

ThreadData threadData[NUM_THREADS];
pthread_t threads[NUM_THREADS];
vector<int> output_global;
int n_global;

void* countingSortThread(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    fill(begin(data->count), end(data->count), 0);

    for (int i = data->start; i < data->end; i++) {
        int digit = ((*data->arr)[i] / data->exp) % 10;
        data->count[digit]++;
    }

    
    pthread_barrier_wait(&barrier);

    return nullptr;
}

void countingSortParallel(vector<int>& arr, int exp) {
    int n = arr.size();
    n_global = n;
    output_global.resize(n);

    int chunkSize = (n + NUM_THREADS - 1) / NUM_THREADS;


    for (int i = 0; i < NUM_THREADS; i++) {
        threadData[i].arr = &arr;
        threadData[i].exp = exp;
        threadData[i].start = i * chunkSize;
        threadData[i].end = min((i + 1) * chunkSize, n);
    }

    pthread_barrier_init(&barrier, nullptr, NUM_THREADS);

    
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], nullptr, countingSortThread, &threadData[i]);
    }

    
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], nullptr);
    }

    
    int count[10] = {0};
    for (int digit = 0; digit < 10; digit++) {
        for (int t = 0; t < NUM_THREADS; t++) {
            count[digit] += threadData[t].count[digit];
        }
    }

    for (int i = 1; i < 10; i++) {
        count[i] += count[i - 1];
    }

    
    for (int i = n - 1; i >= 0; i--) {
        int digit = (arr[i] / exp) % 10;
        output_global[count[digit] - 1] = arr[i];
        count[digit]--;
    }

    arr = output_global;

    pthread_barrier_destroy(&barrier);
}

void radixSortParallel(vector<int>& arr) {
    int maxVal = *max_element(arr.begin(), arr.end());
    for (int exp = 1; maxVal / exp > 0; exp *= 10) {
        countingSortParallel(arr, exp);
    }
}

int main() {
    int n = 1000000;
    vector<int> arr(n);

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, 999999);

    for (int i = 0; i < n; i++) {
        arr[i] = dis(gen);
    }

    cout << "Running Parallel Radix Sort on " << n << " elements...\n";

    auto start = high_resolution_clock::now();
    radixSortParallel(arr);
    auto end = high_resolution_clock::now();

    auto duration = duration_cast<milliseconds>(end - start).count();

    cout << "Execution time (Parallel): " << duration << " ms\n";

    return 0;
}