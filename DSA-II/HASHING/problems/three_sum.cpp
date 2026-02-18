#include "HashTables.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <set>

using namespace std;

void threeSum(vector<int>& arr, int target) {
    int n = arr.size();
    
    // 1. Use a Set to store unique sorted triplets
    // This prevents printing duplicate answers like (1, 2, 3) and (3, 2, 1)
    set<vector<int>> uniqueTriplets;

    cout << "Target Sum: " << target << endl;

    // 2. Fix the first element (i)
    for (int i = 0; i < n - 1; i++) {
        
        int firstNum = arr[i];
        int newTarget = target - firstNum;

        // --- START 2-SUM LOGIC ---
        // We look for pairs in the SUB-ARRAY [i+1 ... end]
        // that sum up to 'newTarget'
        ChainingHashTable<int, int> ht(inthash); 
        int dummyHits = 0;

        for (int j = i + 1; j < n; j++) {
            int secondNum = arr[j];
            int needed = newTarget - secondNum;

            // Check if 'needed' (the 3rd number) is in our hash table
            if (ht.search(needed, dummyHits) != nullptr) {
                // Found a triplet! {firstNum, needed, secondNum}
                vector<int> triplet = {firstNum, needed, secondNum};
                
                // Sort to ensure uniqueness (e.g., {-1, 0, 1} vs {0, -1, 1})
                sort(triplet.begin(), triplet.end());

                // Only print if we haven't seen this exact triplet before
                if (uniqueTriplets.find(triplet) == uniqueTriplets.end()) {
                    cout << "Found Triplet: (" 
                         << triplet[0] << ", " 
                         << triplet[1] << ", " 
                         << triplet[2] << ")" << endl;
                    
                    uniqueTriplets.insert(triplet);
                }
            }

            // Insert current number into Hash Table for future checks
            ht.insert(secondNum, j);
        }
        // --- END 2-SUM LOGIC ---
    }

    if (uniqueTriplets.empty()) {
        cout << "No triplets found." << endl;
    }
}

int main() {
    int n;
    cout << "Enter number of elements: ";
    cin >> n;

    vector<int> arr(n);
    cout << "Enter elements: ";
    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }

    int target;
    cout << "Enter Target: ";
    cin >> target;

    threeSum(arr, target);

    return 0;
}