#include<iostream>
#include<vector>
#include<ctype.h>
#include<queue>
#include<climits>
using namespace std;

class Edge{
    public:
    long long int v;
    long long int w;
    Edge(long long int v,long long int w){
        this->v=v;
        this->w=w;
    }

};

void solve(long long int n, vector<vector<Edge > >& adj, long long int source, long long int student, long long int K, vector<int>&storage){

    vector<pair<long long int, int> > dist(n+1, make_pair(0,0));
    dist[source]=make_pair(0,storage[0]);
    priority_queue<pair<long long int,long long int>,vector<pair<long long int,long long int> >,greater<pair<long long int,long long int> > >pq;
    pq.push(make_pair(0,source));
    while(!pq.empty()){
        auto [d,u] = pq.top(); pq.pop();
        if (d > dist[u].first) continue;

        for(Edge neighbour: adj[u]){
            long long int cost = dist[u].first+neighbour.w;
            if(cost<dist[neighbour.v].first){
                dist[neighbour.v].first=cost;
                dist[neighbour.v].second=storage[neighbour.v];
                pq.push(make_pair(dist[neighbour.v].first,neighbour.v));
            }
        }
    }

    sort(dist.begin(), dist.end(), [](auto &a, auto &b){
        return a.first < b.first; 
    });
    
    int index=1;
    for(int i=0; i<student; i++){
        while(index<=dist.size()){
        if(dist[index].second>0){
            cout<<K+dist[index].first<<endl;
            dist[index].second-=1;
            break;
        }
        else{
            index++;
        }
    }
    
}
}

int main(){
    long long int n,m,k;
    cin>>n>>m>>k;
    vector<vector<Edge> > adj(n+1); 
    vector<int>storage(n+1);
    for(int i=1; i<=n; i++){
        int a; cin>>a;
        storage[i]=a;
    }
    for(long long int i=0; i<m; i++){
        long long int u,v,w;
        cin>>u>>v>>w;
        adj[u].push_back(Edge(v,w));
        adj[v].push_back(Edge(u,w));
    }
     long long int student;
     cin>>student;
     solve(n,adj,1,student,k,storage);
    
}

//g++ -std=c++17 ./previous/dsa_lab.cpp -o c
//./c < ./input.txt