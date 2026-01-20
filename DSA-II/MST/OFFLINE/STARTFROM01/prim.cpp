#include<iostream>
#include<vector>
#include<ctype.h>
#include<queue>
#include<climits>
using namespace std;

void PRIMS(int n, vector<vector<pair<int, int> > >& adj, int source, vector<int>& parent){

    // CHANGE 1: Resize to n + 1 to handle 1-based indexing
    vector<int> dist(n + 1, INT_MAX);
    vector<bool> inMST(n + 1, false);
    
    vector< pair<int,int> > result;
    int total = 0;
    dist[source] = 0;
    
    priority_queue<pair<int,int>, vector<pair<int,int> >, greater<pair<int,int> > > pq;
    pq.push({0, source});
    
    while(!pq.empty()){
        auto [d,u] = pq.top(); pq.pop();
        
        if(d > dist[u]) continue;
        
        inMST[u] = true;
        
        // Ensure parent is valid (source won't have a parent)
        if(parent[u] != -1) result.push_back(make_pair(parent[u], u));
        
        total += d;
        
        for(auto[v,w]: adj[u]){
            if(w < dist[v] && inMST[v] == false){
                dist[v] = w;
                pq.push(make_pair(dist[v], v));
                parent[v] = u;
            }
        }
    }
    cout << "Total weight " << total << endl;
    cout << "Root node " << source << endl; 
    for(auto[u,v] : result){
        cout << u << " " << v << endl;
    }
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m;
    cin >> n >> m;
    
    // CHANGE 2: Resize Adjacency list and Parent vector to n + 1
    vector<vector<pair<int, int> > > adj(n + 1); 
    vector<int> parent(n + 1, -1);
    
    for(int i = 0; i < m; i++){
        int u, v, w;
        cin >> u >> v >> w;
        // Vertices u, v are now safe to use as indices even if they are 'n'
        adj[u].push_back(make_pair(v, w));
        adj[v].push_back(make_pair(u, w));
    }
    int root; cin >> root;
    PRIMS(n, adj, root, parent);
}