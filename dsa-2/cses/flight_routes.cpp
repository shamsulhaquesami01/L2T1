#include<iostream>
#include<vector>
#include<ctype.h>
#include<queue>
#include <climits>
using namespace std;


void solve(int n, vector<vector<pair<int, int> > >& adj, int source, int des,int q){

    vector<int> dist(n+1, INT_MAX);
    dist[source]=0;
    priority_queue<pair<int,int>,vector<pair<int,int> >,greater<pair<int,int> > >pq;
    pq.push(make_pair(0,source));
    while(!pq.empty()){
        auto [d,u] = pq.top(); pq.pop();


        for(auto[v,w]: adj[u]){
            if(dist[u]+w<dist[v]){
                dist[v]=dist[u]+w;
                pq.push(make_pair(dist[v],v));
            }
        }
    }
    if(dist[des]==INT_MAX) cout<<-1<<endl;
    else cout<<dist[des]<<endl;
}
int main(){
    int n,m,q;
    cin>>n>>m;
    vector<vector<pair<int, int> > > adj(n+1); 
    for(int i=0; i<m; i++){
        int u,v,w;
        cin>>u>>v>>w;
        adj[u].push_back(make_pair(v,w));
    }
   
     solve(n,adj,1,n,q);
    
}