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
int main(){
    int n;cin>>n;
    vector<int> arr(n);
    int target;
    cin>>target;
    for(int i=0; i<n; i++){
        cin>>arr[i];
    }
    HashTableBase<int,int>* ht=new ChainingHashTable<int,int>(inthash);
    int dummyhits=0;
    set<pair<int, int>> seenPairs;
    for(int i=0; i<arr.size(); i++){
        int x =arr[i];
        int rest = target+x;
        int* result = ht->search(rest,dummyhits);
        if (result != nullptr) {
          pair<int, int> p = {min(x, rest), max(x, rest)};
            
            // Only print if we haven't seen this pair before
            if (seenPairs.find(p) == seenPairs.end()) {
                cout << "Found pair: (" << rest << " + " << x << ")" << endl;
                seenPairs.insert(p); // Mark as seen
            }
            
        }
        int partner2 = x - target;
        // Check to ensure we don't check the same number twice if target is 0
        if (partner2 != rest && ht->search(partner2, dummyhits) != nullptr) {
            pair<int, int> p = {min(x, partner2), max(x, partner2)};
            if (seenPairs.find(p) == seenPairs.end()) {
                cout << "Found pair: (" << x << ", " << partner2 << ")" << endl;
                seenPairs.insert(p);
            }
        }
        ht->insert(x, i);

    }
return 0;
}