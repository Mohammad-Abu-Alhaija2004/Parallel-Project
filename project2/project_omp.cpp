#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <iomanip>
#include <random>
#include <omp.h>

using namespace std;
using namespace std::chrono;

void countingSortParallel(vector<int>& arr, int exp, int num_threads) {
    int n = arr.size();
    vector<int> output(n);
    vector<vector<int>> local_counts(num_threads, vector<int>(10, 0));

    
    #pragma omp parallel num_threads(num_threads)
    {
        int tid = omp_get_thread_num();
        int chunkSize = (n + num_threads - 1) / num_threads;
        int start = tid * chunkSize;
        int end = min(start + chunkSize, n);

        for (int i = start; i < end; i++) {
            int digit = (arr[i] / exp) % 10;
            local_counts[tid][digit]++;
        }
    }

   
    vector<int> count(10, 0);
    for (int digit = 0; digit < 10; digit++) {
        for (int t = 0; t < num_threads; t++) {
            count[digit] += local_counts[t][digit];
        }
    }

    
    for (int i = 1; i < 10; i++) {
        count[i] += count[i - 1];
    }

    for (int i = n - 1; i >= 0; i--) {
        int digit = (arr[i] / exp) % 10;
        output[count[digit] - 1] = arr[i];
        count[digit]--;
    }

    
    arr = output;
}

void radixSortParallel(vector<int>& arr, int num_threads) {
    int maxVal = *max_element(arr.begin(), arr.end());
    for (int exp = 1; maxVal / exp > 0; exp *= 10) {
        countingSortParallel(arr, exp, num_threads);
    }
}

int main() {
    int n = 1000000;
    int num_threads = 4; 
    vector<int> arr(n);

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, 999999);

    for (int i = 0; i < n; i++) {
        arr[i] = dis(gen);
    }

    cout << "Running Parallel Radix Sort with OpenMP on " << n << " elements using " << num_threads << " threads...\n";

    auto start = high_resolution_clock::now();
    radixSortParallel(arr, num_threads);
    auto end = high_resolution_clock::now();

    auto duration = duration_cast<milliseconds>(end - start).count();

    cout << "Execution time (Parallel OpenMP): " << duration << " ms\n";

    return 0;
}
