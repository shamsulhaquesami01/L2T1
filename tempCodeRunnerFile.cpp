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

// ==========================================
// 1. Prime Number Utilities
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
// 2. Hash Functions
// ==========================================
unsigned long long Hash1(const string& key) {
    unsigned long long hashVal = 0;
    for (char c : key) {
        hashVal = 37 * hashVal + c; 
    }
    return hashVal;
}

unsigned long long Hash2(const string& key) {
    unsigned long long hash = 5381;
    for (char c : key) {
        hash = ((hash << 5) + hash) + c; 
    }
    return hash;
}

unsigned long long AuxHash(const string& key) {
    unsigned long long hash = 0;
    for (char c : key) hash += c;
    return (hash == 0) ? 1 : hash;
}

// ==========================================
// 3. Collision Resolution Enums
// ==========================================
enum ResolutionMethod { CHAINING, DOUBLE_HASHING, CUSTOM_PROBING };

// ==========================================
// 4. Templated Abstract Base Class
// ==========================================
template <typename K, typename V>
class HashTableBase {
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

    virtual void insert(const K& key, V value) = 0;
    
    // UPDATED: Returns Value pointer, outputs hits via reference
    virtual V* search(const K& key, int& hits) = 0; 
    
    virtual void remove(const K& key) = 0;

    long long getCollisions() const { return totalCollisions; }
    
    double getLoadFactor() const { return (double)numElements / tableSize; }

    void checkResize() {
        double load = getLoadFactor();
        
        if (load > 0.5 && insertionsSinceExpand > (numElements / 2)) {
            int newSize = nextPrime(tableSize * 2); 
            rehash(newSize);
            insertionsSinceExpand = 0; 
        }
        else if (load < 0.25 && tableSize > initialSize && deletionsSinceCompact > (numElements / 2)) {
            int newSize = prevPrime(tableSize / 2); 
            if (newSize < initialSize) newSize = initialSize;
            rehash(newSize);
            deletionsSinceCompact = 0; 
        }
    }

    virtual void rehash(int newSize) = 0;
};

// ==========================================
// 5. Templated Chaining Implementation
// ==========================================
template <typename K, typename V>
class ChainingHashTable : public HashTableBase<K, V> {
    vector<list<pair<K, V>>> table;
    unsigned long long (*hashFunc)(const K&); 

public:
    ChainingHashTable(unsigned long long (*hf)(const K&), int size = 13) 
        : HashTableBase<K, V>(size), hashFunc(hf) {
        table.resize(size);
    }

    void insert(const K& key, V value) override {
        unsigned long long h = hashFunc(key);
        int idx = h % this->tableSize;
        
        if (!table[idx].empty()) {
             bool found = false;
             for(auto& p : table[idx]) if(p.first == key) found = true;
             if(!found) this->totalCollisions++;
        }

        for (auto& pair : table[idx]) {
            if (pair.first == key) return; 
        }
        
        table[idx].push_back({key, value});
        this->numElements++;
        this->insertionsSinceExpand++; 
        
        this->checkResize();
    }

    // UPDATED SEARCH
    V* search(const K& key, int& hits) override {
        unsigned long long h = hashFunc(key);
        int idx = h % this->tableSize;
        hits = 0;
        
        hits++; // Access bucket
        
        for (auto& pair : table[idx]) {
            if (pair.first == key) return &pair.second; // Return pointer to value
            hits++;
        }
        return nullptr; // Not found
    }

    void remove(const K& key) override {
        unsigned long long h = hashFunc(key);
        int idx = h % this->tableSize;
        auto& lst = table[idx];
        for (auto it = lst.begin(); it != lst.end(); ++it) {
            if (it->first == key) {
                lst.erase(it);
                this->numElements--;
                this->deletionsSinceCompact++;
                this->checkResize();
                return;
            }
        }
    }

    void rehash(int newSize) override {
        vector<list<pair<K, V>>> oldTable = table;
        table.clear();
        table.resize(newSize);
        this->tableSize = newSize;
        this->numElements = 0; 
        
        for (const auto& lst : oldTable) {
            for (const auto& p : lst) {
                unsigned long long h = hashFunc(p.first);
                int idx = h % this->tableSize;
                
                if (!table[idx].empty()) this->totalCollisions++;

                table[idx].push_back(p);
                this->numElements++;
            }
        }
    }
};

// ==========================================
// 6. Templated Probing Implementation
// ==========================================
enum EntryType { ACTIVE, EMPTY, DELETED };

template <typename K, typename V>
struct HashEntry {
    K key;
    V value;
    EntryType info;
    HashEntry() : info(EMPTY) {}
};

template <typename K, typename V>
class ProbingHashTable : public HashTableBase<K, V> {
    vector<HashEntry<K, V>> table;
    unsigned long long (*hashFunc)(const K&);
    unsigned long long (*auxHashFunc)(const K&);
    
    ResolutionMethod method;
    int C1, C2; 

    int getProbe(const K& key, int i) {
        unsigned long long h = hashFunc(key);
        unsigned long long aux = auxHashFunc(key);
        
        if (aux % this->tableSize == 0) aux = 1;

        unsigned long long idx = 0;

        if (method == DOUBLE_HASHING) {
            idx = (h + i * aux) % this->tableSize;
        } else {
            // Custom: (Hash(k) + C1*i*aux + C2*i*i) % N
            idx = (h + C1 * i * aux + C2 * i * i) % this->tableSize;
        }
        return (int)idx;
    }

public:
    ProbingHashTable(unsigned long long (*hf)(const K&), 
                     unsigned long long (*auxHf)(const K&), 
                     ResolutionMethod m, int size = 13) 
        : HashTableBase<K, V>(size), hashFunc(hf), auxHashFunc(auxHf), method(m), C1(1), C2(1) {
        table.resize(size);
    }

    void insert(const K& key, V value) override {
        int i = 0;
        int idx = getProbe(key, i);
        
        if (table[idx].info == ACTIVE && table[idx].key != key) {
            this->totalCollisions++;
        }

        while (table[idx].info == ACTIVE && table[idx].key != key) {
            i++;
            idx = getProbe(key, i);
            if (i > this->tableSize * 2) break; 
        }

        if (table[idx].key == key && table[idx].info == ACTIVE) return; 

        table[idx].key = key;
        table[idx].value = value;
        table[idx].info = ACTIVE;
        this->numElements++;
        this->insertionsSinceExpand++;
        
        this->checkResize();
    }

    // UPDATED SEARCH
    V* search(const K& key, int& hits) override {
        int i = 0;
        int idx = getProbe(key, i);
        hits = 1; 

        while (table[idx].info != EMPTY) {
            if (table[idx].info == ACTIVE && table[idx].key == key) {
                return &table[idx].value; // Return pointer to value
            }
            i++;
            idx = getProbe(key, i);
            hits++;
            if (i > this->tableSize * 2) break; 
        }
        return nullptr; // Not found
    }

    void remove(const K& key) override {
        int i = 0;
        int idx = getProbe(key, i);
        
        while (table[idx].info != EMPTY) {
            if (table[idx].info == ACTIVE && table[idx].key == key) {
                table[idx].info = DELETED; 
                this->numElements--;
                this->deletionsSinceCompact++;
                this->checkResize();
                return;
            }
            i++;
            idx = getProbe(key, i);
            if (i > this->tableSize * 2) break;
        }
    }

    void rehash(int newSize) override {
        vector<HashEntry<K, V>> oldTable = table;
        table.clear();
        table.resize(newSize);
        this->tableSize = newSize;
        this->numElements = 0;

        for (const auto& entry : oldTable) {
            if (entry.info == ACTIVE) {
                int i = 0;
                int idx = getProbe(entry.key, i); 

                if (table[idx].info == ACTIVE) this->totalCollisions++;
                
                while (table[idx].info == ACTIVE) {
                    i++;
                    idx = getProbe(entry.key, i);
                }

                table[idx].key = entry.key;
                table[idx].value = entry.value;
                table[idx].info = ACTIVE;
                this->numElements++;
            }
        }
    }
};

// ==========================================
// 7. Word Generator
// ==========================================
class WordGenerator {
    int length;
    set<string> generatedWords;
    mt19937 rng;

public:
    WordGenerator(int n) : length(n) {
        rng.seed(time(0));
    }

    string getUniqueWord() {
        string word;
        uniform_int_distribution<int> dist(0, 25);
        do {
            word = "";
            for (int i = 0; i < length; ++i) {
                word += (char)('a' + dist(rng));
            }
        } while (generatedWords.count(word)); 
        
        generatedWords.insert(word);
        return word;
    }
    
    vector<string> getRandomSample(int k) {
        vector<string> allWords(generatedWords.begin(), generatedWords.end());
        shuffle(allWords.begin(), allWords.end(), rng);
        if (k > (int)allWords.size()) k = allWords.size();
        return vector<string>(allWords.begin(), allWords.begin() + k);
    }
};

// ==========================================
// 8. Main Driver & Report
// ==========================================
int main() {
    int N_WORDS = 10000;
    int WORD_LEN = 7;
    int SEARCH_COUNT = 1000;

    cout << "Generating " << N_WORDS << " unique words..." << endl;
    WordGenerator gen(WORD_LEN);
    vector<string> words;
    for(int i=0; i<N_WORDS; i++) {
        words.push_back(gen.getUniqueWord());
    }

    vector<string> searchQueries = gen.getRandomSample(SEARCH_COUNT);

    cout << "--------------------------------------------------------------------------------" << endl;
    cout << setw(20) << "Method" << " | " 
         << setw(25) << "Hash1 (Poly)" << " | " 
         << setw(25) << "Hash2 (DJB2)" << endl;
    cout << setw(20) << " " << " | " 
         << setw(10) << "Collisions" << setw(15) << "Avg Hits" << " | " 
         << setw(10) << "Collisions" << setw(15) << "Avg Hits" << endl;
    cout << "--------------------------------------------------------------------------------" << endl;

    auto runTest = [&](ResolutionMethod method, string methodName) {
        long long col1 = 0, col2 = 0;
        double hit1 = 0, hit2 = 0;

        // --- Run Hash 1 ---
        {
            HashTableBase<string, int>* table;
            if (method == CHAINING) 
                table = new ChainingHashTable<string, int>(Hash1);
            else 
                table = new ProbingHashTable<string, int>(Hash1, AuxHash, method);
            
            for(int i=0; i<N_WORDS; i++) table->insert(words[i], i+1);
            col1 = table->getCollisions();

            long long totalHits = 0;
            for(const string& q : searchQueries) {
                int h = 0;
                table->search(q, h); // Pass 'h' by reference to get hit count
                totalHits += h;
            }
            hit1 = (double)totalHits / SEARCH_COUNT;

            delete table;
        }

        // --- Run Hash 2 ---
        {
            HashTableBase<string, int>* table;
            if (method == CHAINING) 
                table = new ChainingHashTable<string, int>(Hash2);
            else 
                table = new ProbingHashTable<string, int>(Hash2, AuxHash, method);
            
            for(int i=0; i<N_WORDS; i++) table->insert(words[i], i+1);
            col2 = table->getCollisions();

            long long totalHits = 0;
            for(const string& q : searchQueries) {
                int h = 0;
                table->search(q, h);
                totalHits += h;
            }
            hit2 = (double)totalHits / SEARCH_COUNT;

            delete table;
        }

        cout << setw(20) << methodName << " | " 
             << setw(10) << col1 << setw(15) << fixed << setprecision(3) << hit1 << " | " 
             << setw(10) << col2 << setw(15) << fixed << setprecision(3) << hit2 << endl;
    };

    runTest(CHAINING, "Chaining Method");
    runTest(DOUBLE_HASHING, "Double Hashing");
    runTest(CUSTOM_PROBING, "Custom Probing");
    
    cout << "--------------------------------------------------------------------------------" << endl;

    return 0;
}