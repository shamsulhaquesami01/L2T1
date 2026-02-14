#include <iostream>
#include <vector>
#include <list>
#include "HashTables.h" 

using namespace std;

// Node for our order-keeping list
struct OrderNode {
    string key;
    int value;
    OrderNode* next;
    OrderNode* prev;
};

class OrderedMap {
    // 1. The Hash Table stores pointers to the OrderNodes
    ChainingHashTable<string, OrderNode*> ht; 
    
    // 2. The Head/Tail for insertion order
    OrderNode* head = nullptr;
    OrderNode* tail = nullptr;

public:
    OrderedMap() : ht(Hash1) {} // Initialize your table

    void insert(string key, int val) {
        int hits = 0;
        // Check if exists
        OrderNode** existing = ht.search(key, hits);
        
        if (existing) {
            // Update value only
            (*existing)->value = val;
            return;
        }

        // Create new Node
        OrderNode* newNode = new OrderNode{key, val, nullptr, tail};
        
        // Link in DLL
        if (tail) tail->next = newNode;
        else head = newNode;
        tail = newNode;

        // Store POINTER in Hash Table
        ht.insert(key, newNode);
    }

    void printInOrder() {
        OrderNode* curr = head;
        while(curr) {
            cout << curr->key << ": " << curr->value << " -> ";
            curr = curr->next;
        }
        cout << "END" << endl;
    }
};