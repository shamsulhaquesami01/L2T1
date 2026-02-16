#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include "HashTables.h"

using namespace std;

// 1. Helper to generate the Unique Key
string getShiftKey(const string& s) {
    if (s.length() == 0) return "";
    
    stringstream ss;
    // Iterate through adjacent characters
    for (int i = 1; i < s.length(); i++) {
        int diff = (s[i] - s[i-1]);
        
        // Handle negative wrap-around (e.g., 'a' - 'b' = -1 -> 25)
        if (diff < 0) diff += 26;
        
        ss << diff << ","; // Use delimiter to separate numbers
    }
    return ss.str();
}

// 2. Main Solver
void groupShiftedStrings(vector<string>& strs) {
    // Key = Diff Sequence (e.g., "1,1,"), Value = List of original strings
    ChainingHashTable<string, vector<string>> ht(Hash1); 
    int dummyHits = 0;
    
    // To print easily later, we can store keys in a separate list 
    // or assume traverse() exists. I'll use a list for simplicity here.
    vector<string> uniqueKeys; 

    for (string s : strs) {
        string key = getShiftKey(s);
        
        vector<string>* group = ht.search(key, dummyHits);
        
        if (group != nullptr) {
            group->push_back(s);
        } else {
            vector<string> newGroup;
            newGroup.push_back(s);
            ht.insert(key, newGroup);
            uniqueKeys.push_back(key); // Track this new group key
        }
    }

    // 3. Print Groups
    cout << "Shifted Groups:" << endl;
    for (string k : uniqueKeys) {
        vector<string>* group = ht.search(k, dummyHits);
        cout << "[ ";
        for (string s : *group) {
            cout << s << " ";
        }
        cout << "]" << endl;
    }
}

int main() {
    vector<string> input = {"abc", "bcd", "acef", "xyz", "az", "ba", "a", "z"};
    
    groupShiftedStrings(input);

    return 0;
}