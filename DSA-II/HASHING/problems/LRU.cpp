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

// Prime Number Helpers 
bool isPrime(int n)
{
    if (n <= 1)
        return false;
    if (n <= 3)
        return true;
    if (n % 2 == 0 || n % 3 == 0)
        return false;
    for (int i = 5; i * i <= n; i += 6)
    {
        if (n % i == 0 || n % (i + 2) == 0)
            return false;
    }
    return true;
}

int nextPrime(int n)
{
    while (!isPrime(n))
        n++;
    return n;
}

int prevPrime(int n)
{
    while (n > 2 && !isPrime(n))
        n--;
    return n;
}

// Polynomial Rolling Hash
unsigned long long Hash1(const string &key)
{
    unsigned long long hashVal = 0;
    for (char c : key)
    {
        hashVal = 37 * hashVal + c;
    }
    return hashVal;
}

// DJB2 Hash
unsigned long long Hash2(const string &key)
{
    unsigned long long hash = 5381;
    for (char c : key)
    {
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}

unsigned long long AuxHash(const string &key)
{
    unsigned long long hash = 0;
    for (char c : key)
        hash += c;
    return (hash == 0) ? 1 : hash;
}

enum ResolutionMethod
{
    CHAINING,
    DOUBLE_HASHING,
    CUSTOM_PROBING
};

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
    HashEntry():{}
};

template <typename K, typename V>
class ChainingHashTable : public HashTableBase<K, V>
{
    // Now using list of HashEntry instead of pair
    vector<list<HashEntry2<K, V>>> table;
    unsigned long long (*hashFunc)(const K &);
    int time=0;

public:
    ChainingHashTable(unsigned long long (*hf)(const K &), int size = 13)
        : HashTableBase<K, V>(size), hashFunc(hf)
    {
        table.resize(size);
    }

    void insert(const K &key, V value) override
    {
        unsigned long long h = hashFunc(key);
        int idx = h % this->tableSize;

        // Optimization: Single pass to check existence and update
        for (auto &entry : table[idx])
        {
            if (entry.key == key)
            {
                this->time++;
                entry.value = value;
                entry.lastaccesstime=time;
                return;
            }
        }
//lru eviction
if (this->getLoadFactor() > 0.8 && !table[idx].empty()) 
        {
            remove_last(key); // Helper to remove LRU from this bucket
        }
        // If we reach here, the key is new. 
        if (!table[idx].empty())
        {
            this->totalCollisions++;
        }

        HashEntry2<K, V> newEntry;
        newEntry.key = key;
        newEntry.value = value;
        this->time++;
        newentry.lastaccesstime=time;

        table[idx].push_back(newEntry);
        this->numElements++;
        this->insertionsSinceExpand++;

       // this->checkResize();
    }

    V *search(const K &key, int &hits) override
    {
        unsigned long long h = hashFunc(key);
        int idx = h % this->tableSize;
        hits = 0;

        // The first hit is the initial access to the bucket
        hits++; 
        for (auto &entry : table[idx])
        {
            if (entry.key == key){
                this->time++;
                entry.lastaccesstime=this->time;
                return &entry.value;
            }
            hits++;
        }
        return nullptr;
    }

    void remove(const K &key) override
    {
        unsigned long long h = hashFunc(key);
        int idx = h % this->tableSize;
        auto &lst = table[idx];

        for (auto it = lst.begin(); it != lst.end(); ++it)
        {
            if (it->key == key)
            {
                lst.erase(it); // In chaining, we physically remove the node
                this->numElements--;
                this->deletionsSinceCompact++;
                this->checkResize();
                return;
            }
        }
    }

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

        // Erase the LRU element
        // cout << "Evicting LRU Key: " << lru_it->key << endl; // Debug
        lst.erase(lru_it);
        this->numElements--;
    }
    void rehash(int newSize) override
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
                unsigned long long h = hashFunc(entry.key);
                int idx = h % this->tableSize;

                if (!table[idx].empty())
                    this->totalCollisions++;

                table[idx].push_back(entry);
                this->numElements++;
            }
        }
    }
};

enum EntryType
{
    ACTIVE,
    EMPTY,
    DELETED
};

template <typename K, typename V>
struct HashEntry
{
    K key;
    V value;
    EntryType info;
    HashEntry() : info(EMPTY) {}
};


template <typename K, typename V>
class ProbingHashTable : public HashTableBase<K, V>
{
    vector<HashEntry<K, V>> table;
    unsigned long long (*hashFunc)(const K &);
    unsigned long long (*auxHashFunc)(const K &);

    ResolutionMethod method;
    int C1, C2;

    int getProbe(const K &key, int i)
    {
        unsigned long long h = hashFunc(key);
        unsigned long long aux = auxHashFunc(key);

        if (aux % this->tableSize == 0)
            aux = 1;

        unsigned long long idx = 0;

        if (method == DOUBLE_HASHING)
        {
            idx = (h + i * aux) % this->tableSize;
        }
        else
        {
            idx = (h + C1 * i * aux + C2 * i * i) % this->tableSize;
        }
        return (int)idx;
    }

public:
    // Updated Constructor to accept c1 and c2
    ProbingHashTable(unsigned long long (*hf)(const K &),
                     unsigned long long (*auxHf)(const K &),
                     ResolutionMethod m, int c1, int c2, int size = 13)
        : HashTableBase<K, V>(size), hashFunc(hf), auxHashFunc(auxHf), method(m), C1(c1), C2(c2)
    {
        table.resize(size);
    }

    void insert(const K &key, V value) override
    {
        int i = 0;
        int idx = getProbe(key, i);

        while (table[idx].info == ACTIVE && table[idx].key != key)
        {
            this->totalCollisions++;
            i++;
            idx = getProbe(key, i);
            if (i > this->tableSize * 2)
                break;
        }

        if (table[idx].key == key && table[idx].info == ACTIVE)
        {
            table[idx].value = value;
            return;
        }

        table[idx].key = key;
        table[idx].value = value;
        table[idx].info = ACTIVE;
        this->numElements++;
        this->insertionsSinceExpand++;

        this->checkResize();
    }

    V *search(const K &key, int &hits) override
    {
        int i = 0;
        int idx = getProbe(key, i);
        hits = 1;

        while (table[idx].info != EMPTY)
        {
            if (table[idx].info == ACTIVE && table[idx].key == key)
            {
                return &table[idx].value;
            }
            i++;
            idx = getProbe(key, i);
            hits++;
            if (i > this->tableSize * 2)
                break;
        }
        return nullptr;
    }

    void remove(const K &key) override
    {
        int i = 0;
        int idx = getProbe(key, i);

        while (table[idx].info != EMPTY)
        {
            if (table[idx].info == ACTIVE && table[idx].key == key)
            {
                table[idx].info = DELETED;
                this->numElements--;
                this->deletionsSinceCompact++;
                this->checkResize();
                return;
            }
            i++;
            idx = getProbe(key, i);
            if (i > this->tableSize * 2)
                break;
        }
    }

    void rehash(int newSize) override
    {
        vector<HashEntry<K, V>> oldTable = table;
        table.clear();
        table.resize(newSize);
        this->tableSize = newSize;
        this->numElements = 0;

        for (const auto &entry : oldTable)
        {
            if (entry.info == ACTIVE)
            {
                int i = 0;
                int idx = getProbe(entry.key, i);

                while (table[idx].info == ACTIVE)
                {
                    this->totalCollisions++;
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

class WordGenerator
{
    int length;
    set<string> generatedWords;
    mt19937 rng;

public:
    WordGenerator(int n) : length(n)
    {
        rng.seed(time(0));
    }

    string getUniqueWord()
    {
        string word;
        uniform_int_distribution<int> dist(0, 25);
        do
        {
            word = "";
            for (int i = 0; i < length; ++i)
            {
                word += (char)('a' + dist(rng));
            }
        } while (generatedWords.count(word));

        generatedWords.insert(word);
        return word;
    }

    vector<string> getRandomSample(int k)
    {
        vector<string> allWords(generatedWords.begin(), generatedWords.end());
        shuffle(allWords.begin(), allWords.end(), rng);
        if (k > (int)allWords.size())
            k = allWords.size();
        return vector<string>(allWords.begin(), allWords.begin() + k);
    }
};

int main()
{
    int N_WORDS = 10000;
    int WORD_LEN ;
    int SEARCH_COUNT = 1000;
    int C1 , C2;

    cout << "Enter the word length (n): ";
    cin >> WORD_LEN;
    cout << "Enter C1 and C2 for Custom Probing: ";
    cin >> C1 >> C2;

    cout << "Generating " << N_WORDS << " unique words..." << endl;
    WordGenerator gen(WORD_LEN);
    vector<string> words;
    for (int i = 0; i < N_WORDS; i++)
    {
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

    auto runTest = [&](ResolutionMethod method, string methodName)
    {
        long long col1 = 0, col2 = 0;
        double hit1 = 0, hit2 = 0;

        {
            HashTableBase<string, int> *table;
            if (method == CHAINING)
                table = new ChainingHashTable<string, int>(Hash1);
            else
                table = new ProbingHashTable<string, int>(Hash1, AuxHash, method, C1, C2);

            for (int i = 0; i < N_WORDS; i++)
                table->insert(words[i], i + 1);
            col1 = table->getCollisions();

            long long totalHits = 0;
            for (const string &q : searchQueries)
            {
                int h = 0;
                table->search(q, h);
                totalHits += h;
            }
            hit1 = (double)totalHits / SEARCH_COUNT;

            delete table;
        }

        {
            HashTableBase<string, int> *table;
            if (method == CHAINING)
                table = new ChainingHashTable<string, int>(Hash2);
            else
                table = new ProbingHashTable<string, int>(Hash2, AuxHash, method, C1, C2);

            for (int i = 0; i < N_WORDS; i++)
                table->insert(words[i], i + 1);
            col2 = table->getCollisions();

            long long totalHits = 0;
            for (const string &q : searchQueries)
            {
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