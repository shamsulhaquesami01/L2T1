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

vector<string> k_mers(string s,int k){
    vector<string>table;
for(int i=0; i<=s.size()-k;i++){
    string res =s.substr(i,k);
    table.push_back(res);
}
return table;
}

int main(){
    ChainingHashTable<string,int> ht(Hash1);
    int n; 
    cin>>n;
    int k;cin>>k;
    string s;
    cin>>s;
    int hit=0;
    vector<string> table=k_mers(s,k);
    for(string x:table){
        int * fq=ht.search(x,hit);
        if(fq!=nullptr){
            (*fq)++;
        }else{
            ht.insert(x,1);
        }
    }
    ht.printall();
    return 0;
}