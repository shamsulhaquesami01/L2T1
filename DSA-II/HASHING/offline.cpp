/*
 * CSE 208: Hash Table Implementation
 * Constraints Met:
 * 1. Prime Table Size & Dynamic Resizing [cite: 2, 5, 6]
 * 2. Expansion/Contraction constraints (N/2 ops) 
 * 3. Chaining, Double Hashing, Custom Probing [cite: 20, 22, 25]
 * 4. Report Generation [cite: 26, 33]
 */

#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <cmath>
#include <algorithm>
#include <iomanip>
#include <random>
#include <set>

using namespace std;

// ==========================================
// 1. UTILITY & MATH HELPERS
// ==========================================

bool isPrime(int n) {
    if (n <= 1) return false;
    if (n <= 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    for (int i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
    }
    return true;
}

int nextPrime(int n) {
    while (!isPrime(n)) n++;
    return n;
}

int prevPrime(int n) {
    while (n > 2 && !isPrime(n)) n--;
    return n;
}

// ==========================================
// 2. HASH FUNCTIONS 
// ==========================================

// Hash1: Polynomial Rolling Hash
uint64_t Hash1(const string &key) {
    uint64_t hash = 0;
    uint64_t p = 31;
    uint64_t m = 1e9 + 9;
    uint64_t p_pow = 1;
    for (char c : key) {
        hash = (hash + (c - 'a' + 1) * p_pow) % m;
        p_pow = (p_pow * p) % m;
    }
    return hash;
}

// Hash2: Jenkins One-at-a-time Hash (Standard Literature)
uint64_t Hash2(const string &key) {
    uint64_t hash = 0;
    for (char c : key) {
        hash += c;
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    return hash;
}

// ==========================================
// 3. HASH TABLE CLASS
// ==========================================

enum ResolutionMethod { CHAINING, DOUBLE_HASHING, CUSTOM_PROBING };
enum SlotState { EMPTY, OCCUPIED, DELETED }; // For Open Addressing

struct Node {
    string key;
    int value;
    SlotState state;
    Node() : key(""), value(0), state(EMPTY) {}
    Node(string k, int v) : key(k), value(v), state(OCCUPIED) {}
};

class HashTable {
    int tableSize;
    int elementCount;
    int itemsSinceResize; // Tracks operations for the N/2 rule 
    ResolutionMethod method;
    
    // Constants for Custom Probing [cite: 25]
    int C1 = 23, C2 = 17; 

    // Storage for Chaining [cite: 21]
    vector<list<pair<string, int>>> chainTable;

    // Storage for Open Addressing
    vector<Node> probeTable;

public:
    int collisionCount; // To track collisions during insertion [cite: 31]

    HashTable(int size, ResolutionMethod m) {
        tableSize = size; // Must be prime (handled by caller or rehash)
        elementCount = 0;
        itemsSinceResize = 0; // Reset counter
        method = m;
        collisionCount = 0;

        if (method == CHAINING) {
            chainTable.resize(tableSize);
        } else {
            probeTable.resize(tableSize);
        }
    }

    // Helper to clear and reset structure for a new size
    void resetStructure(int newSize) {
        tableSize = newSize;
        elementCount = 0;
        itemsSinceResize = 0; // Reset constraint counter 
        collisionCount = 0;   // Reset stats (optional, usually per-run)
        
        if (method == CHAINING) {
            chainTable.clear();
            chainTable.resize(newSize);
        } else {
            probeTable.clear();
            probeTable.resize(newSize);
        }
    }

    double getLoadFactor() {
        return (double)elementCount / tableSize;
    }

    // Rehash Logic [cite: 5, 6, 7, 8]
    void checkRehash() {
        double lf = getLoadFactor();
        int newSize = tableSize;
        bool needsRehash = false;

        // Expansion Rule: LF > 0.5 AND Insertions >= OldSize/2
        if (lf > 0.5 && itemsSinceResize >= (tableSize / 2)) {
            newSize = nextPrime(tableSize * 2);
            needsRehash = true;
        }
        // Contraction Rule: LF < 0.25 AND Deletions >= OldSize/2
        // Note: initial size is 13, don't shrink below that [cite: 3, 7]
        else if (lf < 0.25 && itemsSinceResize >= (tableSize / 2) && tableSize > 13) {
            newSize = prevPrime(tableSize / 2);
            needsRehash = true;
        }

        if (needsRehash) {
            // Backup current data
            vector<pair<string, int>> oldData;
            if (method == CHAINING) {
                for (auto &lst : chainTable) {
                    for (auto &p : lst) oldData.push_back(p);
                }
            } else {
                for (auto &node : probeTable) {
                    if (node.state == OCCUPIED) oldData.push_back({node.key, node.value});
                }
            }

            // Re-initialize table
            resetStructure(newSize);

            // Re-insert all data (this updates elementCount and hash mappings)
            for (auto &p : oldData) {
                insert(p.first, p.second);
            }
            // IMPORTANT: Re-insertion counts as "setup", not new operations
            itemsSinceResize = 0; 
        }
    }

    // Insertion [cite: 13, 16]
    void insert(string key, int value) {
        checkRehash(); // Check if we need to resize BEFORE inserting

        uint64_t h1 = Hash1(key);
        uint64_t h2 = Hash2(key);
        int idx = 0;
        
        bool collisionEncountered = false;

        if (method == CHAINING) {
            idx = h1 % tableSize;
            // Check for duplicates
            for (auto &p : chainTable[idx]) {
                if (p.first == key) return; // Duplicate discarded [cite: 16]
            }
            if (!chainTable[idx].empty()) collisionCount++; // Stat tracking
            chainTable[idx].push_back({key, value});
            
        } else {
            // Open Addressing
            int i = 0;
            while (i < tableSize) {
                // Calculate Index
                if (method == DOUBLE_HASHING) {
                    // [cite: 22] (H1 + i * H2) % N
                    idx = (h1 + i * h2) % tableSize;
                } else {
                    // [cite: 25] Custom: (H1 + C1*i*H2 + C2*i^2) % N
                    idx = (h1 + C1 * i * h2 + C2 * i * i) % tableSize;
                }

                if (probeTable[idx].state == OCCUPIED) {
                    if (probeTable[idx].key == key) return; // Duplicate
                    collisionCount++; // Stat tracking
                    collisionEncountered = true;
                } else {
                    // Found EMPTY or DELETED slot
                    probeTable[idx] = Node(key, value);
                    break;
                }
                i++;
            }
        }
        elementCount++;
        itemsSinceResize++;
    }

    // Search: Returns Hits (probes required) or -1 if not found [cite: 32]
    int search(string key) {
        uint64_t h1 = Hash1(key);
        uint64_t h2 = Hash2(key);
        int idx = 0;
        int probes = 0;

        if (method == CHAINING) {
            idx = h1 % tableSize;
            for (auto &p : chainTable[idx]) {
                probes++;
                if (p.first == key) return probes;
            }
            return -1; // Not found
        } else {
            int i = 0;
            while (i < tableSize) {
                probes++;
                if (method == DOUBLE_HASHING) {
                    idx = (h1 + i * h2) % tableSize;
                } else {
                    idx = (h1 + C1 * i * h2 + C2 * i * i) % tableSize;
                }

                if (probeTable[idx].state == EMPTY) return -1; // Never existed
                if (probeTable[idx].state == OCCUPIED && probeTable[idx].key == key) {
                    return probes;
                }
                // If DELETED, we continue probing
                i++;
            }
        }
        return -1;
    }

    void remove(string key) {
        // Not strictly needed for the stats report, but required by spec [cite: 1]
        // Implementation logic mirrors Search, but marks DELETED or removes from list
        // Decrement elementCount and increment itemsSinceResize
        uint64_t h1 = Hash1(key);
        uint64_t h2 = Hash2(key);
        int idx;

        if (method == CHAINING) {
            idx = h1 % tableSize;
            auto it = chainTable[idx].begin();
            while (it != chainTable[idx].end()) {
                if (it->first == key) {
                    chainTable[idx].erase(it);
                    elementCount--;
                    itemsSinceResize++; // Treated as "deletion op" for resizing logic
                    checkRehash(); // Check if we need to shrink
                    return;
                }
                it++;
            }
        } else {
             int i = 0;
            while (i < tableSize) {
                if (method == DOUBLE_HASHING) idx = (h1 + i * h2) % tableSize;
                else idx = (h1 + C1 * i * h2 + C2 * i * i) % tableSize;

                if (probeTable[idx].state == EMPTY) return;
                if (probeTable[idx].state == OCCUPIED && probeTable[idx].key == key) {
                    probeTable[idx].state = DELETED; // Tombstone
                    elementCount--;
                    itemsSinceResize++;
                    checkRehash();
                    return;
                }
                i++;
            }
        }
    }
};

// ==========================================
// 4. RANDOM GENERATOR [cite: 12]
// ==========================================

string generateRandomWord(int n) {
    string word = "";
    for (int i = 0; i < n; i++) {
        word += (char)('a' + rand() % 26);
    }
    return word;
}

// ==========================================
// 5. MAIN DRIVER & REPORT [cite: 26, 33]
// ==========================================

int main() {
    srand(time(0));
    int N = 7; // Length of random words [cite: 11]
    int TOTAL_WORDS = 10000;
    int SEARCH_COUNT = 1000;

    // 1. Generate Unique Words [cite: 14]
    vector<string> uniqueWords;
    set<string> wordSet;
    while (uniqueWords.size() < TOTAL_WORDS) {
        string w = generateRandomWord(N);
        if (wordSet.find(w) == wordSet.end()) {
            wordSet.insert(w);
            uniqueWords.push_back(w);
        }
    }

    // 2. Select Search Targets [cite: 28]
    vector<string> searchTargets;
    for(int i=0; i<SEARCH_COUNT; i++) {
        searchTargets.push_back(uniqueWords[rand() % TOTAL_WORDS]);
    }

    cout << "----------------------------------------------------------------" << endl;
    cout << setw(20) << "Method" << " | " 
         << setw(15) << "Collisions" << " | " 
         << setw(15) << "Avg Hits" << endl;
    cout << "----------------------------------------------------------------" << endl;

    // We run the experiment for Chaining, Double Hashing, Custom Probing
    ResolutionMethod methods[] = {CHAINING, DOUBLE_HASHING, CUSTOM_PROBING};
    string names[] = {"Chaining", "Double Hashing", "Custom Probing"};

    for (int m = 0; m < 3; m++) {
        // Initialize Table [cite: 3, 4]
        HashTable ht(13, methods[m]); 

        // Insert Words [cite: 26]
        for (int i = 0; i < TOTAL_WORDS; i++) {
            ht.insert(uniqueWords[i], i + 1);
        }

        // Search Words & Calc Average Hits [cite: 29]
        long totalHits = 0;
        for (string s : searchTargets) {
            totalHits += ht.search(s);
        }
        double avgHits = (double)totalHits / SEARCH_COUNT;

        cout << setw(20) << names[m] << " | " 
             << setw(15) << ht.collisionCount << " | " 
             << setw(15) << fixed << setprecision(3) << avgHits << endl;
    }

    cout << "----------------------------------------------------------------" << endl;
    return 0;
}


