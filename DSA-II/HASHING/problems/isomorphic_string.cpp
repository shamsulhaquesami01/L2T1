#include "HashTables.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Simple Char Hash
unsigned long long CharHash(const char& key) { return (unsigned long long)key; }

bool checkIsomorphic(string s, string t) {
    if (s.length() != t.length()) return false;

    ChainingHashTable<char, char> s_to_t(CharHash); // Map S chars to T chars
    ChainingHashTable<char, char> t_to_s(CharHash); // Map T chars to S chars
    int dummyHits = 0;

    for (int i = 0; i < s.length(); i++) {
        char c1 = s[i];
        char c2 = t[i];

        char* mappedTo = s_to_t.search(c1, dummyHits);
        char* mappedFrom = t_to_s.search(c2, dummyHits);

        // Case 1: c1 has been seen before
        if (mappedTo != nullptr) {
            if (*mappedTo != c2) return false; // Conflict!
        } else {
            // Case 2: c1 is new, but c2 has been used by someone else
            if (mappedFrom != nullptr) return false;

            // Create new mapping
            s_to_t.insert(c1, c2);
            t_to_s.insert(c2, c1);
        }
    }
    return true;
}

int main() {
    int Q;
    cin >> Q;
    while(Q--) {
        string s, t;
        cin >> s >> t;
        if (checkIsomorphic(s, t)) cout << s << " " << t << ": Yes" << endl;
        else cout << s << " " << t << ": No" << endl;
    }
    return 0;
}