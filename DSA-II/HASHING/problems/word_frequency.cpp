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

int main() {
    ChainingHashTable<string, int> freqMap(Hash1);
    string word;
    int dummy = 0;

    // 1. Read until END
    cout << "Enter words (type 'END' to stop):" << endl;
    while (cin >> word && word != "END") {
        // Optional: Convert to lowercase here
        int* count = freqMap.search(word, dummy);
        if (count) (*count)++;
        else freqMap.insert(word, 1);
    }

    int K;
    cout << "Enter K: ";
    cin >> K;

    // 2. Get the sorted list directly from the class!
    vector<pair<string, int>> topWords = freqMap.exportSortedByValue();

    // 3. Print the first K (or fewer if list is small)
    int limit = min(K, (int)topWords.size());
    
    cout << "Top " << limit << " Words:" << endl;
    for (int i = 0; i < limit; i++) {
        cout << topWords[i].first << ": " << topWords[i].second << endl;
    }

    return 0;
}