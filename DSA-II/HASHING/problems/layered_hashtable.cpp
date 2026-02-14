
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
// --- Helper Hash Functions ---
void printpairs(const int& key, const string& value) {
     cout<<"user id: "<<key<<" group id:"<<value<<endl;
}
// 1. Hash Functions (Simple Modulo/Multiplication for integers)
unsigned long long Hash_1(const int& key) {
    return (unsigned long long)key; 
}

unsigned long long Hash_2(const int& key) {
    return (unsigned long long)key * 2654435761; // Knuth's Multiplicative Hash
}

// 2. AuxHash for Linear Probing (Must return 1)
unsigned long long AuxLinear(const int& key) {
    return 1;
}

// 3. AuxHash for Quadratic (Not used because C1=0, but needed for constructor)
unsigned long long AuxQuadratic(const int& key) {
    return 1; 
}

// Type Definitions for readability
// Inner Table: Key=int (user_id), Value=string (perm)
typedef ProbingHashTable<int, string> InnerTable;

// Outer Table: Key=int (group_id), Value=Pointer to InnerTable
typedef ProbingHashTable<int, InnerTable*> OuterTable;


int main() {
    int N, Q;
    cin >> N >> Q;

    // --- Configure Outer Table (Linear Probing) ---
    // Method: CUSTOM_PROBING, C1=1, C2=0, AuxHash=AuxLinear (Returns 1)
    // Formula becomes: (Hash + 1*i*1 + 0) = Hash + i
    OuterTable outerDB(Hash_1, AuxLinear, CUSTOM_PROBING, 1, 0, N);

    while(Q--) {
        string command;
        cin >> command;

        if (command == "INSERT") {
            int group_id, user_id;
            string perm;
            cin >> group_id >> user_id >> perm;

            int dummyHits = 0;
            // 1. Search for existing Group in Outer Table
            // Search returns a pointer to the Value (which is InnerTable*)
            InnerTable** innerPtrPtr = outerDB.search(group_id, dummyHits);

            if (innerPtrPtr != nullptr) {
                // Group Exists: Insert into existing inner table
                InnerTable* innerDB = *innerPtrPtr;
                innerDB->insert(user_id, perm);
            } 
            else {
                // New Group: Create new Inner Table
                // Inner Table uses Quadratic Probing (C1=0, C2=1)
                InnerTable* newInnerDB = new InnerTable(Hash_2, AuxQuadratic, CUSTOM_PROBING, 0, 1, N);
                
                // Insert User into new Inner Table
                newInnerDB->insert(user_id, perm);

                // Insert (Group, InnerTable*) into Outer Table
                outerDB.insert(group_id, newInnerDB);
            }
        }
        else if (command == "SEARCH") {
            int group_id;
            cin >> group_id;
            int dummyHits = 0;
            // Check if user_id is provided (Lookahead)
            if (cin.peek() != '\n' && cin.peek() != EOF) {
                 // SEARCH <group_id> <user_id>
                 int user_id;
                 cin >> user_id;
                 
                 
                 InnerTable** innerPtrPtr = outerDB.search(group_id, dummyHits);
                 
                 if (innerPtrPtr == nullptr) {
                     cout << "Group not found" << endl;
                 } else {
                     InnerTable* innerDB = *innerPtrPtr;
                     string* perm = innerDB->search(user_id, dummyHits);
                     if(perm) cout << *perm << endl;
                     else cout << "User not found in group " << group_id << endl;
                 }
            } 
            else {
                // SEARCH <group_id> (Print All)
                // This requires the traverse function we added earlier!
                // If you don't have traverse, you cannot implement this part generically easily.
                cout << "Printing all users for group " << group_id << "..." << endl;
                  InnerTable** innerPtrPtr = outerDB.search(group_id, dummyHits);
                 
                 if (innerPtrPtr == nullptr) {
                     cout << "Group not found" << endl;
                 } else {
                     InnerTable* innerDB = *innerPtrPtr;
                     innerDB->traverse(printpairs);
                 }
                
            }
        }
        else if (command == "DELETE") {
            int group_id, user_id;
            cin >> group_id >> user_id;

            int dummyHits = 0;
            InnerTable** innerPtrPtr = outerDB.search(group_id, dummyHits);

            if (innerPtrPtr != nullptr) {
                InnerTable* innerDB = *innerPtrPtr;
                string* perm = innerDB->search(user_id, dummyHits);
                if (perm) {
                    cout << "(" << user_id << ", " << *perm << ") deleted" << endl;
                    innerDB->remove(user_id);
                }
            }
        }
    }
    return 0;
}