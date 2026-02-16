#include <iostream>
#include <vector>
#include <string>

using namespace std;

// --- Entry Structure ---
struct CuckooEntry {
    int key;
    string value;
    bool isActive;

    // 1. Default Constructor (Used when vector resizes)
    CuckooEntry() : key(0), value(""), isActive(false) {}

    // 2. Parameterized Constructor (Used for your insertion line)
    CuckooEntry(int k, string v, bool active) : key(k), value(v), isActive(active) {}
};
class CuckooHashTable {
    vector<CuckooEntry> table1;
    vector<CuckooEntry> table2;
    int currentSize;
    int numElements;
    
    // Limits
    const int MAX_KICKS_LIMIT = 50; 

public:
    CuckooHashTable(int size) : currentSize(size), numElements(0) {
        table1.resize(size);
        table2.resize(size);
    }

    // --- 1. Two Independent Hash Functions ---
    int hash1(int key) {
        return (key % currentSize);
    }

    int hash2(int key) {
        return (key / currentSize) % currentSize;
    }

    // --- 2. Search is O(1) ---
    string search(int key) {
        int pos1 = hash1(key);
        if (table1[pos1].isActive && table1[pos1].key == key)
            return table1[pos1].value;

        int pos2 = hash2(key);
        if (table2[pos2].isActive && table2[pos2].key == key)
            return table2[pos2].value;

        return ""; // Not Found
    }

    // --- 3. The Insert Logic (Recursive or Iterative) ---
    void insert(int key, string value) {
        // Check if already exists to update
        if (search(key) != "") {
            cout << "Update logic not implemented for brevity." << endl;
            return;
        }

        // Prepare the item we want to insert
        int currKey = key;
        string currVal = value;

        // Try to place it, kicking others if needed
        for (int count = 0; count < MAX_KICKS_LIMIT; count++) {
            
            // --- Attempt Table 1 ---
            int pos1 = hash1(currKey);
            if (!table1[pos1].isActive) {
                // Found empty spot!
                table1[pos1] = {currKey, currVal, true};
                numElements++;
                return;
            }

            // Conflict! Kick out existing item in Table 1
            swap(currKey, table1[pos1].key);
            swap(currVal, table1[pos1].value);
            // Now 'currKey' holds the VICTIM we just kicked out.

            // --- Attempt Table 2 ---
            int pos2 = hash2(currKey);
            if (!table2[pos2].isActive) {
                // Found empty spot for victim!
                table2[pos2] = {currKey, currVal, true};
                numElements++;
                return;
            }

            // Conflict in Table 2! Kick out item there.
            swap(currKey, table2[pos2].key);
            swap(currVal, table2[pos2].value);
            
            // Loop continues... we try to put this new victim back into Table 1
        }

        // If we reach here, we exceeded MAX_KICKS. Cycle detected.
        cout << "Cycle detected! Rehash required." << endl;
        rehash();
        // Retry insertion after rehash
        insert(key, value); 
    }

    // --- 4. Rehash (Double Size) ---
    void rehash() {
        cout << "Rehashing from size " << currentSize << " to " << currentSize * 2 << endl;
        
        // Save old data
        vector<CuckooEntry> oldT1 = table1;
        vector<CuckooEntry> oldT2 = table2;

        // Resize
        currentSize *= 2;
        table1.clear(); table1.resize(currentSize);
        table2.clear(); table2.resize(currentSize);
        numElements = 0; // Reset count, insert will increment it back

        // Re-insert everything
        for (auto &e : oldT1) 
            if (e.isActive) insert(e.key, e.value);
            
        for (auto &e : oldT2) 
            if (e.isActive) insert(e.key, e.value);
    }

    void printTable() {
        cout << "--- Table 1 ---" << endl;
        for (int i = 0; i < currentSize; i++) {
            if (table1[i].isActive) cout << "[" << i << "]: " << table1[i].key << endl;
        }
        cout << "--- Table 2 ---" << endl;
        for (int i = 0; i < currentSize; i++) {
            if (table2[i].isActive) cout << "[" << i << "]: " << table2[i].key << endl;
        }
    }
};

int main() {
    CuckooHashTable ht(5);

    ht.insert(5, "A"); // T1[0]
    ht.insert(10, "B"); // T1[0] occupied! Kick 5 -> T2.
    ht.insert(15, "C"); // T1[0] occupied! Kick 10 -> T2 -> Kick 5 -> T1...
    
    ht.printTable();

    return 0;
}