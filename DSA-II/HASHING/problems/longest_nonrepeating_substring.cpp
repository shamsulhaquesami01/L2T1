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


unsigned long long CharHash(const char& key) {
    return (unsigned long long)key;
}

int lengthOfLongestSubstring(string s) {
    // Key = Character, Value = Index where it was last seen
    ChainingHashTable<char, int> ht(CharHash, 26); // Size 26 or 128 is good for chars
    
    int left = 0;
    int maxLength = 0;
    int dummyHits = 0;

    for (int right = 0; right < s.length(); right++) {
        char currentChar = s[right];

        // 1. Check if char exists in table
        int* lastSeenPtr = ht.search(currentChar, dummyHits);

        // 2. Collision Logic
        if (lastSeenPtr != nullptr) {
            int lastSeenIndex = *lastSeenPtr;
            
            // CRITICAL CHECK: Is the duplicate inside our current window?
            // If lastSeenIndex < left, it's an old "ghost" outside the window. Ignore it.
            if (lastSeenIndex >= left) {
                left = lastSeenIndex + 1; // Jump past the duplicate
            }
        }

        // 3. Update the character's latest index
        // Your insert function updates the value if key exists
        ht.insert(currentChar, right);

        // 4. Calculate max length
        // +1 because length = difference + 1 (e.g., indices 2 to 2 has length 1)
        maxLength = max(maxLength, right - left + 1);
    }

    return maxLength;
}

int main(){
    string s;
    cin>>s;
    cout<<lengthOfLongestSubstring(s);
}