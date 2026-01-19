#include<iostream>
#include<vector>
#include<ctype.h>
#include<queue>
#include<climits>
using namespace std;
void PRIMS(int n, vector<vector<pair<int, int> > >& adj, int source,vector<int>& parent){

    vector<int> dist(n, INT_MAX);
    vector< pair<int,int> > result;
    vector<bool>inMST(n,false);
    int total =0;
    dist[source]=0;
    priority_queue<pair<int,int>,vector<pair<int,int> >,greater<pair<int,int> > >pq;
    pq.push({0, source});
    while(!pq.empty()){
        auto [d,u] = pq.top(); pq.pop();
        if(d>dist[u]) continue;
        inMST[u]=true;
        if(parent[u]!=-1)result.push_back(make_pair(parent[u],u));
        total+=d;
        for(auto[v,w]: adj[u] ){
            if(w<dist[v] && inMST[v] == false){
                dist[v]=w;
                pq.push(make_pair(dist[v],v));
                parent[v]=u;
            }
        }
    }
    cout<<"Total weight "<<total<<endl;
    cout << "Root node " << source << endl; 
    for(auto[u,v] : result){
        cout<<u<<" "<<v<<endl;
    }
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n,m;
    cin>>n>>m;
    vector<vector<pair<int, int> > > adj(n); 
    vector<int> parent(n, -1);
    for(int i=0; i<m; i++){
        int u,v,w;
        cin>>u>>v>>w;
        adj[u].push_back(make_pair(v,w));
        adj[v].push_back(make_pair(u,w));
    }
    int root; cin>>root;
     PRIMS(n,adj,root,parent);
    
}


// g++ -std=c++17 b_sec.cpp -o solution && ./solution < input.txt