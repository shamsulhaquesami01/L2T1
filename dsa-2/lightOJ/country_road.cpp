#include<iostream>
#include<vector>
#include<ctype.h>
#include<queue>
using namespace std;

// void dijkstra(int source){
//     vector<vector<pair<int, int> > > adj; 
//     vector<int> dist(n, INT_MAX);
//     dist[source]=0;
//     priority_queue<pair<int,int>,vector<pair<int,int> >,greater<pair<int,int> > >pq;
//     pq.push(make_pair(0,source));
//     while(!pq.empty()){
//         auto [d,u] = pq.top(); pq.pop();


//         for(auto[v,w]: adj[u]){
//             if(dist[u]+w<dist[v]){
//                 dist[v]=dist[u]+w;
//                 pq.push(make_pair(dist[v],v));
//             }
//         }
//     }
// }
void solve(){
        int n,m,k;
    cin>>n>>m;
    vector<vector<pair<int, int> > > adj(n); 
    vector<int> dist(n, INT_MAX);
    
    for(int i=0; i<m ;i++){
    int u,v,w;
    cin>>u>>v>>w;
    adj[u].push_back(make_pair(v,w));
    adj[v].push_back(make_pair(u,w));
    }
    int source;
    cin>>source;
    dist[source]=0;
    priority_queue<pair<int,int>,vector<pair<int,int> >,greater<pair<int,int> > >pq;
    pq.push(make_pair(0,source));
    while(!pq.empty()){
        auto [d,u] = pq.top(); pq.pop();


        for(auto[v,w]: adj[u]){
            if(dist[u]>w){
            if(dist[u]<dist[v]){
                dist[v]=dist[u];
                pq.push(make_pair(dist[v],v));
            }
        }
        else{
            if(w<dist[v]){
                dist[v]=w;
               pq.push(make_pair(dist[v],v));
            }
        }
        }
    }
    for(int i=0; i<n; i++) {
        if(dist[i] >20000) cout<<"Impossible"<<endl;
        else cout<<dist[i]<<endl;
    }
}
int main(){
    
  int t ;
  cin>>t;
  for(int i=0;i<t;i++){
    cout<<"Case "<<i+1<<":"<<endl;
    solve();
  }

}