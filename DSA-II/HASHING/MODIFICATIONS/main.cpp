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