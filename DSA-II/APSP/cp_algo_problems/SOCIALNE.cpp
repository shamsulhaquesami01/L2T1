#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <climits>
using namespace std;
using ll = long long;
const ll INF = 1e18;

vector<ll>  bfs(int start,int n, vector<vector<int> > &adj){
   vector<ll> dist(n+1, INF);
   dist[start] =0;
   queue<ll> q;
   q.push(start);
   while(!q.empty()){
    ll u = q.front(); q.pop();
    for(int v:adj[u]){
     if (dist[v] == INF) { // only visit if not visited yet 
    dist[v] = dist[u] + 1; q.push(v); 
}
    }
   }
   return dist;
}
int main(){
    int n;
    cin>>n;
    vector<vector<int> > adj(n+1);
    for(int i=0; i<n; i++){
        for(int j=0;j<n; j++){
            char c; cin>>c;
            if(c=='Y'){
                adj[i].push_back(j);
                adj[j].push_back(i);
            }
        }
    }
    int ans=0;
    ll node= INF;
    for(int i=0; i<n; i++){
        int count =0;
        vector<ll> dist = bfs(i,n,adj);
        for(int i :dist){
            if (i==2) count++;
        }
       if(count> ans){
        ans =count;
        node =i;
       }
    }
    cout<<"Node No:"<<node<<endl;
    cout<<"possible friends:"<<ans<<endl;
    
}
//g++ -std=c++17 dsa-2/APSP/cp_algo_problems/SOCIALNE.cpp -o solution && ./solution < dsa-2/APSP/OFFLINE/input.txt 