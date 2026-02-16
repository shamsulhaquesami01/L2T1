#include <iostream>
#include <vector>
#include "HashTables.h"

using namespace std;

// Standard Integer Hash Function
unsigned long long IntHash(const int& key) {
    return (unsigned long long)key; // Simple cast usually works for int keys
}

int subarraySum(vector<int>& nums, int k) {
    // Key = Prefix Sum, Value = Frequency (How many times seen)
    ChainingHashTable<int, int> ht(IntHash); 
    
    int currentSum = 0;
    int count = 0;
    int dummyHits = 0;

    // IMPORTANT: Initialize with {0: 1}
    // This handles the case where a subarray starts strictly from index 0
    ht.insert(0, 1);

    for (int num : nums) {
        currentSum += num;
        int needed = currentSum - k;

        // 1. Check if (currentSum - k) exists in the table
        int* freqPtr = ht.search(needed, dummyHits);
        
        if (freqPtr != nullptr) {
            // If found, we add the frequency to our count
            count += *freqPtr;
        }

        // 2. Insert/Update currentSum in the table
        int* currFreqPtr = ht.search(currentSum, dummyHits);
        if (currFreqPtr != nullptr) {
            (*currFreqPtr)++; // Increment existing frequency
        } else {
            ht.insert(currentSum, 1); // First time seeing this sum
        }
    }

    return count;
}

int main() {
    // Example 1: Standard
    vector<int> nums1 = {1, 1, 1};
    int k1 = 2;
    cout << "Input: {1, 1, 1}, K=2 -> Count: " << subarraySum(nums1, k1) << endl; 
    // Subarrays: [1,1] (index 0-1), [1,1] (index 1-2) -> Output: 2

    // Example 2: Negative Numbers
    vector<int> nums2 = {1, 2, 3, -3};
    int k2 = 3;
    cout << "Input: {1, 2, 3, -3}, K=3 -> Count: " << subarraySum(nums2, k2) << endl;
    // Subarrays: [1,2], [3], [1,2,3,-3] -> Output: 3

    return 0;
}