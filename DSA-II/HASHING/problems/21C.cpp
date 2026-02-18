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
    
    int idx=0;
    while(true){
        int n; 
    cin>>n;
    if(n==1){
        string name; string nid; string phone;
        cin>>name>>nid>>phone;
        ht.insert(name,idx);
         ht.insert(nid,idx);
          ht.insert(phone,idx);
        idx++;
    }
    if(n==2){
        string x;
        cin>>x;
        int hit =0;
        int * idx=ht.search(x,hit);
        if(idx != nullptr){
           vector<string> res= ht.getKeysByValue(*idx);
           cout<<"info :"<<endl;
           for(string s:res){
            cout<<s<<" ";
           }
        }
        else{
            cout<<"no match foubnd"<<endl;
        }
    }
}
    return 0;
}