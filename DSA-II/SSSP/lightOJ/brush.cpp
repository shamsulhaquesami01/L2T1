#include<iostream>
#include<vector>
#include<ctype.h>
#include<queue>
#include <climits>
using namespace std;
void solve(){
    int n,m;
    cin>>n>>m;
    vector<int> dist(n+1, INT_MAX);
    vector<vector<pair<int,int> > > adj(n+1);
    for(int i=0; i<m; i++){
      int u,v,w;
      cin>>u>>v>>w;
      adj[u].push_back(make_pair(v,w));
      adj[v].push_back(make_pair(u,w));
    }
    dist[1]=0;
    priority_queue< pair<int,int>, vector<pair<int,int> >, greater<pair<int,int> > >pq;
    pq.push(make_pair(0,1));
    while(!pq.empty()){
        auto [d,u] = pq.top(); pq.pop();
        if(d > dist[u]) continue;
        for(auto[v,w]: adj[u]){
             if(dist[u]+w<dist[v]){
              dist[v]=dist[u]+w;
              pq.push(make_pair(dist[v],v));
             }
        }
    }
      
    if(dist[n] == INT_MAX) cout << "Impossible\n";
    else cout << dist[n] << "\n";
    }


int main(){
    
  int t ;
  cin>>t;
  for(int i=0;i<t;i++){
    cout<<"Case "<<i+1<<": ";
    solve();
  }

}