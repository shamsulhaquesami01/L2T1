#include "HashTables.h"
#include <iostream>
#include <vector>

using namespace std;

void findZeroSumSubarray() {
    int N;
    cin >> N;
    vector<int> arr(N);
    for(int i=0; i<N; i++) cin >> arr[i];

    // Key = Prefix Sum, Value = Index where this sum occurred
    ChainingHashTable<int, int> map(inthash);
    
    int currentSum = 0;
    int dummy = 0;
    int start = -1, end = -1;

    for (int i = 0; i < N; i++) {
        currentSum += arr[i];

        // Case 1: Sum is exactly 0
        if (currentSum == 0) {
            start = 0;
            end = i;
            break;
        }

        // Case 2: Sum seen before
        int* prevIndex = map.search(currentSum, dummy);
        if (prevIndex != nullptr) {
            start = *prevIndex + 1;
            end = i;
            break;
        }

        // Store current sum and index
        map.insert(currentSum, i);
    }

    if (start != -1) {
        cout << "YES Subarray: ";
        for (int i = start; i <= end; i++) {
            cout << arr[i] << " ";
        }
        cout << endl;
    } else {
        cout << "NO" << endl;
    }
}

int main() {
    findZeroSumSubarray();
    return 0;
}