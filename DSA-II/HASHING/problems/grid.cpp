#include "HashTables.h"
#include <iostream>
#include <string>
 
using namespace std;
 
// Helper to convert coordinate to string key: (10, 20) -> "10,20"
string pointToKey(int x, int y) {
    return to_string(x) + "," + to_string(y);
}
 
void countDistinctPoints() {
    int N;
    cin >> N;
    
    // Key = "x,y", Value = Count
    ChainingHashTable<string, int> gridMap(Hash1);
    int dummy = 0;
 
    for (int i = 0; i < N; i++) {
        int x, y;
        cin >> x >> y;
        
        string key = pointToKey(x, y);
        
        int* count = gridMap.search(key, dummy);
        if (count) {
            (*count)++;
        } else {
            gridMap.insert(key, 1);
        }
    }
 
    // Use your exportSortedByValue to see most frequent locations
    vector<pair<string, int>> points = gridMap.exportSortedByValue();
    
    cout << "Distinct Points: " << points.size() << endl;
    for(auto p : points) {
        cout << "(" << p.first << ") -> Count: " << p.second << endl;
    }
}