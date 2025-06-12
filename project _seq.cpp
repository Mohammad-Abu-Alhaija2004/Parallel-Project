#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <iomanip>
#include <random>

using namespace std;
using namespace std::chrono;


int getMax(const vector<int>& arr) {
    return *max_element(arr.begin(), arr.end());
}

void countingSort(vector<int>& arr, int exp) {
    int n = arr.size();
    vector<int> output(n);
    int count[10] = {0};

    for (int i = 0; i < n; i++)
        count[(arr[i] / exp) % 10]++;

    for (int i = 1; i < 10; i++)
        count[i] += count[i - 1];

    for (int i = n - 1; i >= 0; i--) {
        int digit = (arr[i] / exp) % 10;
        output[count[digit] - 1] = arr[i];
        count[digit]--;
    }

    
    arr = output;
}

void radixSortSequential(vector<int>& arr) {
    int maxVal = getMax(arr);
    for (int exp = 1; maxVal / exp > 0; exp *= 10) {
        countingSort(arr, exp);
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

    cout << "Running Sequential Radix Sort on " << n << " elements...\n";

    auto start = high_resolution_clock::now();
    radixSortSequential(arr);
    auto end = high_resolution_clock::now();

    auto duration = duration_cast<milliseconds>(end - start).count();

    cout << "Execution time (Sequential): " << duration << " ms\n";

    return 0;
}