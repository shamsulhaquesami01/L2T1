
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
void printAnagramGroup(const string& key, const vector<string>& group) {
    // Optional: Only print if there are actual anagrams (more than 1 word)
    if (group.size() >= 1) { 
        cout << "Key [" << key << "]: ";
        for (const string& word : group) {
            cout << word << " ";
        }
        cout << endl;
    }
}
int main(){
    
    vector<string> strs;
    // Key = Sorted String, Value = List of original strings
    // We use vector<string> as the Value type
    ChainingHashTable<string, vector<string>> ht(Hash1);
    int dummyHits = 0;

    for(string s : strs) {
        // 1. Generate Key (Sort the string)
        string key = s;
        sort(key.begin(), key.end());

        // 2. Search if key exists
        vector<string>* group = ht.search(key, dummyHits);

        if(group != nullptr) {
            // Found existing group, add this word
            group->push_back(s);
            // Update table (Conceptually; pointers allow direct mod if strictly impl, 
            // but standard insert pattern is safer)
            ht.insert(key, *group); 
        } else {
            // New group
            vector<string> newGroup;
            newGroup.push_back(s);
            ht.insert(key, newGroup);
        }
    }

    ht.traverse(printAnagramGroup);
}