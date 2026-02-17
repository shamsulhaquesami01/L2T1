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

unsigned long long inthash(const int& key) {
    // Cast to unsigned int internally for bitwise operations if needed
    unsigned int ukey = (unsigned int)key; 
    
    ukey = (ukey ^ 61) ^ (ukey >> 16);
    ukey = ukey * 2654435769U;
    return ukey ^ (ukey >> 16);
}
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
 
    void traverse(void (*callback)(const K&, const V&)) {
    for (const auto& bucket : table) {
        for (const auto& pair : bucket) {
            callback(pair.first, pair.second);
        }
    }
}
    void hashOrder(){
        int i=0;
        for(auto bucket:table){
            cout<<"bucket "<<i<<" ";
            for(auto p:bucket){
                    
                    cout<<p.first<<" ";
            }
            cout<<endl;
            i++;
        }
    }
    int bucket_length(int index){
        int count =0;
          for(auto p:table[index]){
                count++;
            }
            return count;
    }
    void collision_stats(){
        int ln0=0,ln1=0,ln2=0,ln3=0,ln4=0;
        int total=0;
        int highest=-1;
        for(int idx=0; idx<table.size(); idx++){
        int lngth=bucket_length(idx);
        highest=max(highest,lngth);
        total += lngth;
        if(lngth==0)ln0++;
        if(lngth==1)ln1++;
        if(lngth==2)ln2++;
        if(lngth==3)ln3++;
        if(lngth>=4)ln4++;
       }
       cout<<"Bucket 0 elements: "<<ln0<<endl;
       cout<<"Bucket 1 elements: "<<ln1<<endl;
       cout<<"Bucket 2 elements: "<<ln2<<endl;
       cout<<"Bucket 3 elements: "<<ln3<<endl;
       cout<<"Bucket 4+ elements: "<<ln4<<endl;
       cout<<"total collisions: "<<this->getCollisions()<<endl;
       cout<<"avg chain length: "<<(double)total/this->tableSize<<endl;
       cout<<"longest chain: "<<highest<<endl;

    }
vector<V> rangeQuery(const K& start, const K& end) {
    vector<V> result;
    
    for (int i = 0; i < this->tableSize; i++) {
        for (const auto& pair : table[i]) {
            if (pair.first >= start && pair.first <= end) {
                result.push_back(pair.second);
            }
        }
    }
    
    return result;
}
  
vector<V> rangeQuerySorted(const K& start, const K& end) {
       vector<pair<K, V>> temp;
        
    // Collect all pairs in range
      for (int i = 0; i < this->tableSize; i++) {
          for (const auto& pair : table[i]) {
              if (pair.first >= start && pair.first <= end) {
                  temp.push_back(pair);
                }
            }
        }
        
        // Sort by key
        sort(temp.begin(), temp.end(), 
             [](const pair<K, V>& a, const pair<K, V>& b) {
                 return a.first < b.first;
            });
        
        // Extract values
        vector<V> result;
        for (const auto& pair : temp) {
           result.push_back(pair.second);
        }
        
        return result;
    }

vector<K> getKeysInBucket(int index){
    vector<K> result;
    for(auto p:table[index]){
        result.push_back(p.first);
    }
    return result;
}
    vector<pair<K,V> > exportSorted(){
        vector<pair<K, V>> temp;
        
    // Collect all pairs in range
      for (int i = 0; i < this->tableSize; i++) {
          for (const auto& pair : table[i]) {
              
                  temp.push_back(pair);
               
            }
        }
        
         sort(temp.begin(), temp.end(), 
             [](const pair<K, V>& a, const pair<K, V>& b) {
                 return a.first < b.first;
            });
    return temp;
    }

// Add this inside public: of ChainingHashTable
vector<pair<K, V>> exportSortedByValue() {
    vector<pair<K, V>> temp;

    // 1. Collect all pairs
    for (int i = 0; i < this->tableSize; i++) {
        for (const auto& pair : table[i]) {
            temp.push_back(pair);
        }
    }

    // 2. Sort by VALUE (Count) in Descending Order
    // If counts are equal, sort by Key (Alphabetical) as a tie-breaker
    sort(temp.begin(), temp.end(), 
         [](const pair<K, V>& a, const pair<K, V>& b) {
             if (a.second != b.second) {
                 return a.second > b.second; // High count first
             }
             return a.first < b.first; // A-Z tie-breaker
         });

    return temp;
}

void unionprint(){
      for(auto bucket:table){
            for(auto p:bucket){
                    
                    if(p.second==0||p.second==1||p.second==-1) cout<<p.first<<" ";
            }
        }
}
void untersectprint(){
      for(auto bucket:table){
            for(auto p:bucket){
                    
                    if(p.second==0) cout<<p.first<<" ";
            }
        }
}
void diffprint(){
      for(auto bucket:table){
            for(auto p:bucket){
                    
                    if(p.second==1) cout<<p.first<<" ";
            }
        }
}
int isWellDistributed() {

        int total=0;
        int violation=-1;
        for(int idx=0; idx<table.size(); idx++){
        int lngth=bucket_length(idx);
        total += lngth;
       }
       double avg =(double)total/this->tableSize;
       cout<<"avg chain length: "<<avg<<endl;
        for(int idx=0; idx<table.size(); idx++){
        if(bucket_length(idx)>2*avg) violation++;
       }
       //cout<<table.size()<<endl;
       return violation;
}

void swapValues(K key1, K key2){
    int dummyhit=0;
    V * val1= this->search(key1,dummyhit);
    V * val2=this->search(key2,dummyhit);
    if (val1 == nullptr || val2 == nullptr) {
        cout << "Swap failed: One or both keys not found." << endl;
        return; 
    }
    V temp = *val1;
    *val1=*val2;
    *val2=temp;
}
vector<K> getKeysByValue(const V& targetValue) {
    vector<K> result;
    
    for (int i = 0; i < this->tableSize; i++) {
        for (const auto& pair : table[i]) {
            if (pair.second == targetValue) {
                result.push_back(pair.first);
            }
        }
    }
    
    return result;
}

double getImbalance(){
    double avg = this->getLoadFactor();
    double res=0;
    for(int idx=0; idx<this->tableSize;idx++){
        int len= this->bucket_length(idx);
        double ans = (len-avg)*(len-avg);
        res+=ans;
    }
    return res;
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
        
    int K_LIMIT = 5; // Example bound
    if (table[idx].size() > K_LIMIT) {
        // Force expansion even if load factor is low
        this->rehash(nextPrime(this->tableSize * 2));
    }
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
 // Add inside ProbingHashTable class
int getMaxClusterSize() {
    int maxCluster = 0;
    int currentCluster = 0;
    
    // 1. Standard Linear Scan
    for(int i = 0; i < table.size(); i++) {
        if(table[i].info == ACTIVE) {
            currentCluster++;
        } else {
            maxCluster = max(maxCluster, currentCluster);
            currentCluster = 0;
        }
    }
    maxCluster = max(maxCluster, currentCluster); 

    // 2. Handle Wrap-Around Case (Circular Array)
    if(table[0].info == ACTIVE && table[table.size()-1].info == ACTIVE) {
        int headCluster = 0;
        for(int i = 0; i < table.size() && table[i].info == ACTIVE; i++) 
            headCluster++;
            
        int tailCluster = 0;
        for(int i = table.size() - 1; i >= 0 && table[i].info == ACTIVE; i--) 
            tailCluster++;
            
        if(headCluster + tailCluster >= table.size()) 
            return table.size();
            
        maxCluster = max(maxCluster, headCluster + tailCluster);
    }

    return maxCluster;
}

int getMaxDisplacement(){
    int maximum=-1;
    for(auto p:table){
        if(p.info==ACTIVE){
           int current_idx =idx_search(p.key);
           int main_idx=getProbe(p.key,0);
           maximum=max(maximum,(current_idx-main_idx+this->tableSize)%this->tableSize);
        }
    }
   return maximum;
}
int idx_search(const K &key) 
    {
        int i = 0;
        int idx = getProbe(key, i);

        while (table[idx].info != EMPTY)
        {
            if (table[idx].info == ACTIVE && table[idx].key == key)
            {
                return idx;
            }
            i++;
            idx = getProbe(key, i);
            if (i > this->tableSize * 2)
                break;
        }
        return -1;
}

void traverse(void (*callback)(const K&, const V&)) {
    for (const auto& entry : table) {
        if (entry.info == ACTIVE) {
            callback(entry.key, entry.value);
        }
    }
}

void hashOrder(){
   for(int idx=0; idx<table.size(); idx++){
    cout<<"index "<<idx;
    if(table[idx].info==ACTIVE){
        cout<<table[idx].key<<" ";
    }
    cout<<endl;
   }
}
 void printprobe(const K& key)  {
      
        int i = 0;
        int idx = getProbe(key, i);
        
        if(table[idx].info == EMPTY){
            cout<<idx<<" ";
            return;
        }
        while (table[idx].info != EMPTY)
        {
            cout<<idx<<" ";
            if (table[idx].info == ACTIVE && table[idx].key == key)
            {
                return;
            }
            i++;
            idx = getProbe(key, i);
            if (i > this->tableSize * 2)
                break;
        }
        cout<<idx<<" ";
    }

 int probelength(const K& key)  {
      
        int i = 0;
        int idx = getProbe(key, i);
        
        if(table[idx].info == EMPTY){
           return i+1;
        }
        while (table[idx].info != EMPTY)
        {
            
            if (table[idx].info == ACTIVE && table[idx].key == key)
            {
                return i+1;
            }
            i++;
            idx = getProbe(key, i);
            if (i > this->tableSize * 2)
                break;
        }
        return i+1;
    
    }
int maxProbe(){
    int maximum=-1;
    for(int idx=0; idx<table.size(); idx++){
    if(table[idx].info==ACTIVE){
        maximum=max(maximum,probelength(table[idx].key));
    }
}
   return maximum;
}
    vector<V> rangeQuery(const K& start, const K& end) {
      vector<V> result;
      
   // Iterate through entire table
       for (int i = 0; i < this->tableSize; i++) {
          // Only check ACTIVE entries
         if (table[i].info == ACTIVE) {
               // Check if key is in range
           if (table[i].key >= start && table[i].key <= end) {
            result.push_back(table[i].value);
        }
      }
        }
      
       return result;
    }

int printKeysHashingTo(int targetIndex) {
    int count=0;
    for(int idx =0; idx<this->tableSize; idx++){
        if(table[idx].info==ACTIVE ){
            int h=getProbe(table[idx].key,0);
             if (h == targetIndex) {
                    cout << table[idx].key << " ";
                    count++;
                }
        }
    }
    return count;
}
K keyWIthMaxCOllision(){
    int maximum=-1;
    K mkey ;
    for(int idx=0; idx<table.size(); idx++){
    if(table[idx].info==ACTIVE){
        int len = probelength(table[idx].key);
        if(len>maximum){
            maximum=len;
            mkey=table[idx].key;
        }
    }
}
   return mkey;
}

void swapValues(K key1, K key2){
    int dummyhit=0;
    V * val1= this->search(key1,dummyhit);
    V * val2=this->search(key2,dummyhit);
    V temp = *val1;
    *val1=*val2;
    *val2=temp;
}

vector<K> getKeysByValue(const V& targetValue) {
    vector<K> result;
    
    for (int i = 0; i < this->tableSize; i++) {
        if (table[i].info == ACTIVE) {
            if (table[i].value == targetValue) {
                result.push_back(table[i].key);
            }
        }
    }
    
    return result;
}
void clearDeletedMarkers() {
    vector<pair<K, V>> activeEntries;
    
    // Collect ACTIVE entries
    for (int i = 0; i < this->tableSize; i++) {
        if (table[i].info == ACTIVE) {
            activeEntries.push_back({table[i].key, table[i].value});
        }
    }
    
    // Clear entire table
    for (int i = 0; i < this->tableSize; i++) {
        table[i].info = EMPTY;
    }
    
    // Reinsert all active entries
    //numElements = 0;
    for (const auto& entry : activeEntries) {
        table.insert(entry.first, entry.second);
    }
    
    cout << "Cleared all DELETED markers" << endl;
    cout << "Reinserted " << activeEntries.size() << " entries" << endl;
}

vector<K> findOrphanedKeys() {
vector<K> res;
for(int idx=0; idx<this->tableSize;idx++){
    if(table[idx].info==ACTIVE){
        int h=getProbe(table[idx].key,0);
        if(h!=idx) res.push_back(table[idx].key);
    }
}
return res;
}

void findAllCollidingPairs() {
    // 1. Create a temporary map to group keys by their HOME address
    // Index = Hash Code (0 to tableSize-1)
    // Value = List of keys that wanted this address
    vector<vector<K>> homeBuckets(this->tableSize);

    // 2. Iterate through the ACTUAL table to find all active keys
    for (int i = 0; i < this->tableSize; i++) {
        if (table[i].info == ACTIVE) {
            // Calculate where this key ORIGINALLY wanted to be
            int homeIndex = getProbe(table[i].key, 0);
            
            // Add it to that home bucket
            homeBuckets[homeIndex].push_back(table[i].key);
        }
    }

    // 3. Print pairs from buckets that have more than 1 key
    cout << "Colliding Key Pairs (Primary Hash):" << endl;
    bool foundAny = false;

    for (int i = 0; i < this->tableSize; i++) {
        if (homeBuckets[i].size() > 1) {
            foundAny = true;
            cout << "Index " << i << ": ";
            
            // Print all keys in this bucket
            for (size_t j = 0; j < homeBuckets[i].size(); j++) {
                cout << homeBuckets[i][j];
                if (j < homeBuckets[i].size() - 1) cout << ", ";
            }
            cout << " (Count: " << homeBuckets[i].size() << ")" << endl;
        }
    }

    if (!foundAny) cout << "No collisions found." << endl;
}

vector<K> getKeysVisitingIndex(int targetIndex) {
    vector<K> visitors;

    // Iterate over every slot in the table
    for (int i = 0; i < this->tableSize; i++) {
        
        // We only care about keys that are actually in the table
        if (table[i].info == ACTIVE) {
            K currentKey = table[i].key;
            
            // --- REPLAY PROBE SEQUENCE ---
            int step = 0;
            int probeIdx = getProbe(currentKey, step);
            
            while (true) {
                // 1. Did this key step on the target index?
                if (probeIdx == targetIndex) {
                    visitors.push_back(currentKey);
                    break; // Found it! Move to the next key.
                }

                // 2. Did we reach the key's ACTUAL location?
                // If we reached index 'i' (where the key lives now) and 
                // still haven't seen targetIndex, we never will.
                if (probeIdx == i) {
                    break; 
                }

                // 3. Move to next step in the sequence
                step++;
                probeIdx = getProbe(currentKey, step);

                // Safety break (infinite loop protection)
                if (step > this->tableSize) break;
            }
        }
    }
    return visitors;
}
//basic functions
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
