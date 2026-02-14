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
int main(){
    vector<int> arr(4);
    int target;
    cin>>target;
    for(int i=0; i<3; i++){
        cin>>arr[i];
    }
    HashTableBase<int,int>* ht=new ChainingHashTable<int,int>(inthash);
    int dummyhits=0;
    for(int i=0; i<arr.size(); i++){
        int x =arr[i];
        int rest = target-x;
        int* result = ht->search(rest,dummyhits);
        if (result != nullptr) {
            cout << "Found pair: Indices " << *result << " and " << i 
                 << " (" << rest << " + " << x << ")" << endl;
            return 0;
        }

        ht->insert(x, i);

    }
cout<<"no pair"<<endl;
return 0;
}