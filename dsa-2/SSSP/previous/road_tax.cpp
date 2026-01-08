#include <iostream>
#include <vector>
#include <climits> 

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

void solve(int n, vector<vector<Edge > >& adj, int source, int des){

    vector<int> dist(n+1, INT_MAX);
    dist[source]=0;
    priority_queue<pair<int,int>,vector<pair<int,int> >,greater<pair<int,int> > >pq;
    pq.push(make_pair(0,source));
    while(!pq.empty()){
        auto [d,u] = pq.top(); pq.pop();

        if (d<dist[u]) continue;
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
    int n,m;
    cin>>n>>m;
    vector<int>tax(n+1);
    vector<vector<Edge> >adj(n+1);
    for(int i=1; i<=n; i++){
        int c; cin>>c;
        tax[i]=c;
    }
    for(int i=1; i<=m; i++){
        int u,v; cin>>u>>v;
        adj[u].push_back(Edge(v,tax[v]));
        adj[v].push_back(Edge(u,tax[u]));
    }
    solve(n,adj,1,n);

}