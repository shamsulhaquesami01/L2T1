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

int main()
{
    int N_WORDS = 10000;
    int WORD_LEN=10 ;
    int SEARCH_COUNT = 1000;
    int C1 =2, C2=3;

    // cout << "Enter the word length (n): ";
    // cin >> WORD_LEN;
    // cout << "Enter C1 and C2 for Custom Probing: ";
    // cin >> C1 >> C2;

    cout << "Generating " << N_WORDS << " unique words..." << endl;
    WordGenerator gen(WORD_LEN);
    vector<string> words;
    for (int i = 0; i < N_WORDS; i++)
    {
        words.push_back(gen.getUniqueWord());
    }

    vector<string> searchQueries = gen.getRandomSample(SEARCH_COUNT);

            ProbingHashTable<string, int> *table;
            table = new ProbingHashTable<string, int>(Hash1,AuxHash,CUSTOM_PROBING,C1,C2);
            // ChainingHashTable<string, int> *table;
            // table = new ChainingHashTable<string, int>(Hash1);
            for (int i = 0; i < N_WORDS; i++)
                table->insert(words[i], i + 1);
    //cout<<table->getMaxDisplacement()<<endl;
    // vector<string> res= table->getKeysInBucket(1);
    // for(string x:res) cout<<x<<endl;
    //cout<<table->isWellDistributed();
    // int hit=0;
    // cout<<*(table->search(words[0],hit))<<"-- "<<*(table->search(words[1],hit))<<endl;
    // table->swapValues(words[0],words[1]);
    // cout<<*(table->search(words[0],hit))<<"-- "<<*(table->search(words[1],hit))<<endl;
    vector<string> st= table->findOrphanedKeys();
    for(auto s:st) cout<<s<<endl;
    return 0;
}


//g++ -std=c++17 main.cpp -o a && ./a