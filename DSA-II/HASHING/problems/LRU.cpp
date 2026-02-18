
#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <algorithm>

using namespace std;

// --- 1. Fixed HashEntry Structure ---
unsigned long long Hash1(const string &key)
{
    unsigned long long hashVal = 0;
    for (char c : key)
    {
        hashVal = 37 * hashVal + c;
    }
    return hashVal;
}

template <typename K, typename V>
class HashTableBase
{
protected:
    int tableSize;
    int numElements;
    int initialSize;

    int insertionsSinceExpand;
    int deletionsSinceCompact;

    long long totalCollisions;

public:
    HashTableBase(int size = 13) : tableSize(size), initialSize(size),
                                   numElements(0), insertionsSinceExpand(0),
                                   deletionsSinceCompact(0), totalCollisions(0) {}

    virtual ~HashTableBase() {}

    virtual void insert(const K &key, V value) = 0;
    virtual V *search(const K &key, int &hits) = 0;
    virtual void remove(const K &key) = 0;

    long long getCollisions() const { return totalCollisions; }
    double getLoadFactor() const { return (double)numElements / tableSize; }

    void checkResize()
    {
        double load = getLoadFactor();

        if (load > 0.5 && insertionsSinceExpand > (numElements / 2))
        {
            int newSize = nextPrime(tableSize * 2);
            rehash(newSize);
            insertionsSinceExpand = 0;
        }
        else if (load < 0.25 && tableSize > initialSize && deletionsSinceCompact > (numElements / 2))
        {
            int newSize = prevPrime(tableSize / 2);
            if (newSize < initialSize)
                newSize = initialSize;
            rehash(newSize);
            deletionsSinceCompact = 0;
        }
    }

    virtual void rehash(int newSize) = 0;
};

template <typename K, typename V>
struct HashEntry2
{
    K key;
    V value;
    int lastaccesstime;

    // FIX: Proper Constructor Syntax
    HashEntry2(K k = K(), V v = V(), int t = 0) 
        : key(k), value(v), lastaccesstime(t) {}
};

// --- 2. Updated ChainingHashTable with LRU Logic ---
template <typename K, typename V>
class ChainingHashTable : public HashTableBase<K, V>
{
    // Using list of HashEntry2 (Key, Value, Time)
    vector<list<HashEntry2<K, V>>> table;
    unsigned long long (*hashFunc)(const K &);
    // Global clock to track access order
    int time = 0; 

public:
    ChainingHashTable(unsigned long long (*hf)(const K &), int size = 13)
        : HashTableBase<K, V>(size), hashFunc(hf)
    {
        table.resize(size);
    }

    void insert(const K &key, V value) 
    {
        unsigned long long h = hashFunc(key);
        int idx = h % this->tableSize;
        
        // Increment global time on every operation
        this->time++;

        // 1. Update Existing Key (Hit)
        for (auto &entry : table[idx])
        {
            if (entry.key == key)
            {
                entry.value = value;
                entry.lastaccesstime = this->time; // Update Access Time (LRU Logic)
                return; 
            }
        }

        // 2. Eviction Logic (Bucket-Level LRU)
        // If this specific bucket is getting crowded or table is heavy,
        // we remove the LRU item FROM THIS BUCKET to make room.
        // (Note: This is a localized LRU strategy, not global)
        int K_BUCKET_LIMIT = 3; // Example: Max 3 items per bucket
        if (table[idx].size() >= K_BUCKET_LIMIT || this->getLoadFactor() > 0.8) 
        {
            if (!table[idx].empty()) {
                cout << "[Evict] Bucket " << idx << " full. Removing LRU item..." << endl;
                remove_last(key); 
            }
        }

        // 3. Insert New Key
        // FIX: Corrected variable name 'newentry' -> 'newEntry'
        HashEntry2<K, V> newEntry(key, value, this->time);
        
        table[idx].push_back(newEntry);
        
        this->numElements++;
        this->insertionsSinceExpand++;

        // Optional: Standard Resize
        // this->checkResize(); 
    }

    V *search(const K &key, int &hits) 
    {
        unsigned long long h = hashFunc(key);
        int idx = h % this->tableSize;
        hits = 0;

        this->time++; // Search also counts as an "Access"

        for (auto &entry : table[idx])
        {
            hits++;
            if (entry.key == key) {
                // Critical: Update timestamp on read!
                entry.lastaccesstime = this->time; 
                return &entry.value;
            }
        }
        return nullptr;
    }

    void remove(const K &key) 
    {
        unsigned long long h = hashFunc(key);
        int idx = h % this->tableSize;
        auto &lst = table[idx];

        for (auto it = lst.begin(); it != lst.end(); ++it)
        {
            if (it->key == key)
            {
                lst.erase(it);
                this->numElements--;
                this->deletionsSinceCompact++;
                return;
            }
        }
    }

    // Helper: Remove the Least Recently Used item from the SPECIFIC bucket
    void remove_last(const K &key) {
        unsigned long long h = hashFunc(key);
        int idx = h % this->tableSize;
        auto &lst = table[idx];

        if (lst.empty()) return;

        // Find the entry with the MINIMUM lastaccesstime
        auto lru_it = lst.begin();
        for (auto it = lst.begin(); it != lst.end(); ++it) {
            if (it->lastaccesstime < lru_it->lastaccesstime) {
                lru_it = it;
            }
        }

        cout << " -> Evicted Key: " << lru_it->key << " (Time: " << lru_it->lastaccesstime << ")" << endl;
        
        lst.erase(lru_it);
        this->numElements--;
    }

    void rehash(int newSize) 
    {
        vector<list<HashEntry2<K, V>>> oldTable = table;
        table.clear();
        table.resize(newSize);
        this->tableSize = newSize;
        this->numElements = 0;

        for (const auto &lst : oldTable)
        {
            for (const auto &entry : lst)
            {
                // Re-inserting preserves the old timestamps
                insert_internal(entry.key, entry.value, entry.lastaccesstime);
            }
        }
    }

    // Helper for rehash to preserve timestamps
    void insert_internal(K key, V value, int timestamp) {
        unsigned long long h = hashFunc(key);
        int idx = h % this->tableSize;
        table[idx].push_back(HashEntry2<K, V>(key, value, timestamp));
        this->numElements++;
    }

    // Debug Function to visualize LRU status
    void printCacheState() {
        cout << "\n--- Cache State (Current Time: " << this->time << ") ---" << endl;
        for(int i=0; i<tableSize; i++) {
            if(!table[i].empty()) {
                cout << "Bucket " << i << ": ";
                for(auto e : table[i]) {
                    cout << "[" << e.key << ":" << e.value << " (T=" << e.lastaccesstime << ")] -> ";
                }
                cout << "NULL" << endl;
            }
        }
        cout << "------------------------------------------\n" << endl;
    }
};

// --- 3. Main Function for Testing LRU ---
int main() {
    // Small table size (5) to force collisions and evictions
    ChainingHashTable<string, int> cache(Hash1, 5);
    
    cout << "1. Inserting A, B, C..." << endl;
    cache.insert("A", 100); // Time 1
    cache.insert("B", 200); // Time 2
    cache.insert("C", 300); // Time 3`
    cache.printCacheState();

    cout << "2. Accessing 'A' (Updating its timestamp)..." << endl;
    int hits=0;
    cache.search("A", hits); // Time 4 (A becomes most recent)
    cache.printCacheState();

    cout << "3. Inserting Collision Items (D, E) to trigger eviction..." << endl;
    // Assuming Hash1 maps some of these to the same bucket as A/B/C
    // If bucket limit is 3, inserting a 4th item into a bucket triggers eviction
    cache.insert("D", 400); 
    cache.insert("E", 500); 
    cache.insert("F", 600); // Should trigger eviction in crowded buckets
    
    cache.printCacheState();

    return 0;
}