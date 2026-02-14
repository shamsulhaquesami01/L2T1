#ifndef HASHTABLES_H
#define HASHTABLES_H

#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <set>
#include <random>
#include <algorithm>
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
class ChainingHashTable : public HashTableBase<K, V>
{
    vector<list<pair<K, V>>> table;
    unsigned long long (*hashFunc)(const K &);

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

        if (!table[idx].empty())
        {
            bool found = false;
            for (auto &p : table[idx])
                if (p.first == key)
                    found = true;

            if (!found)
                this->totalCollisions++;
        }

        for (auto &pair : table[idx])
        {
            if (pair.first == key)
            {
                pair.second = value;
                return;
            }
        }

        table[idx].push_back({key, value});
        this->numElements++;
        this->insertionsSinceExpand++;

        this->checkResize();
    }

    V *search(const K &key, int &hits) override
    {
        unsigned long long h = hashFunc(key);
        int idx = h % this->tableSize;
        hits = 0;

        hits++;
        for (auto &pair : table[idx])
        {
            if (pair.first == key)
                return &pair.second;
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
            if (it->first == key)
            {
                lst.erase(it);
                this->numElements--;
                this->deletionsSinceCompact++;
                this->checkResize();
                return;
            }
        }
    }

    void rehash(int newSize) override
    {
        vector<list<pair<K, V>>> oldTable = table;
        table.clear();
        table.resize(newSize);
        this->tableSize = newSize;
        this->numElements = 0;

        for (const auto &lst : oldTable)
        {
            for (const auto &p : lst)
            {
                unsigned long long h = hashFunc(p.first);
                int idx = h % this->tableSize;

                if (!table[idx].empty())
                    this->totalCollisions++;

                table[idx].push_back(p);
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
    int getmaxClusturesize(){
        
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


#endif
