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
unsigned long long charhash(const char& key){
    return key;
}
using namespace std;
int main(){
  ChainingHashTable<char,int> ht(charhash);
  string s;
  cin>>s;
  int hit=0;
  for(char c :s){
    int* freq= ht.search(c,hit);
    if(freq!=nullptr){
        (*freq)++;
    }
    else{
        ht.insert(c,1);
    }
  }

  for (char c : s) {
        int* countPtr = ht.search(c, hit);
        if (*countPtr == 1) {
            cout<< c; 
            break;
        }
    }

return 0;
}

//g++ -std=c++17 first_nonrepeating.cpp -o a && ./a