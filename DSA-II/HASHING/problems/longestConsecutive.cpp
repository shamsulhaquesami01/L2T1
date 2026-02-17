#include "HashTables.h"
#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <random>
#include <set>
#include <ctime>

using namespace std;

int longestConsecutive(vector<int>& nums) {
    // 1. Populate Hash Table
    ChainingHashTable<int, int> ht(inthash); // Use your int hash function
    for (int x : nums) {
        ht.insert(x, 1); // Value doesn't matter, using as Set
    }

    int maxLen = 0;
    int dummyHits = 0;

    // 2. Iterate and Count
    for (int x : nums) {
        // Optimization: Only start counting if 'x' is the START of a sequence.
        // If (x-1) is in the table, then 'x' is just a middle piece. Skip it.
        if (ht.search(x - 1, dummyHits) == nullptr) {
            
            int currentNum = x;
            int currentStreak = 1;

            // Keep checking for x+1, x+2...
            while (ht.search(currentNum + 1, dummyHits) != nullptr) {
                currentNum += 1;
                currentStreak += 1;
            }

            maxLen = max(maxLen, currentStreak);
        }
    }

    return maxLen;
}