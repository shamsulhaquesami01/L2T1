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

string shifting(string s){
string res;
for(int i=s.size()-1; i>=0; i--){
    if(s[i]=='A') res+='T';
    if(s[i]=='T') res+='A';
    if(s[i]=='G') res+='C';
    if(s[i]=='C') res+='G';
}
return res;
}

int main(){
    ChainingHashTable<string,int> ht(Hash1);
    int n; 
    cin>>n;
    vector<string> table;
    for(int i=0; i<n; i++){
        string x;cin>>x;
            table.push_back(x);
    }
    int i=0;
    for(string s:table){
        
        string shifted= shifting(s); int hit=0;
        int *gp= ht.search(shifted,hit);
        if(gp!=nullptr){
            cout<<s<<" "<<shifted<<endl;
        }
        ht.insert(s,i);
        i++;
    }
    return 0;
}