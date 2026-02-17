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
    
            ChainingHashTable<int, int> *table;
            table = new ChainingHashTable<int, int>(inthash);
            int hit=0;
            int sizeA;cin>>sizeA;
            for(int i=0; i<sizeA;i++){
                int x; 
                cin>>x;
                int* freq=table->search(x,hit);
                if(freq !=nullptr){
                    (*freq)++;
                }
                else{
                    table->insert(x,1);
                }
            }
            int sizeB;cin>>sizeB;
            for(int i=0; i<sizeB;i++){
                int x; 
                cin>>x;
                int* freq=table->search(x,hit);
                if(freq !=nullptr){
                    (*freq)--;
                }
                else{
                    table->insert(x,-1);
                }
            }
            cout<<"union "<<endl;
            table->unionprint();
            cout<<endl<<" inter :"<<endl;
            table->untersectprint();
            cout<<endl<<" diff :"<<endl;
            table->diffprint();
   
}


// g++ -std=c++17 C_sec.cpp -o a && ./a