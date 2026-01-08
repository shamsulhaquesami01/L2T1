#include<iostream>
#include<vector>
#include<ctype.h>
#include<queue>
#include<climits>
using namespace std;
class Edge{
    public:
    long long int v;
    long long int time;
    long long int w;
    Edge(long long int v, long long int time, long long int w){
        this->time=time;
        this->v=v;
        this->w=w;
    }

};
void solve(long long int n, vector<vector<Edge > >& adj, long long int source, long long int des, long long int K){

    vector<long long int> dist(n+1, LLONG_MAX);
    vector<int> parent(n+1, -1);
    dist[source]=0;
    priority_queue<pair<long long int,long long int>,vector<pair<long long int,long long int> >,greater<pair<long long int,long long int> > >pq;
    pq.push(make_pair(0,source));
    while(!pq.empty()){
        auto [d,u] = pq.top(); pq.pop();
        if (d > dist[u]) continue;

        for(Edge neighbour: adj[u]){
            long long int cost = dist[u]+neighbour.w+K*neighbour.time;
            if(u != source) cost +=K;
            if(cost<dist[neighbour.v]){
                parent[neighbour.v]=u;
                dist[neighbour.v]=cost;
                pq.push(make_pair(dist[neighbour.v],neighbour.v));
            }
        }
    }
    if(dist[des]==LLONG_MAX) cout<<"Error"<<endl;
    else{
        vector<int> path;
        int curr = des;
        while (curr != -1) {
            path.push_back(curr);
            curr = parent[curr];
        }
        reverse(path.begin(), path.end());

        // Calculate Total Time
        long long totalTime = 0;
        if (path.size() > 2) totalTime += (path.size() - 2); // Wait times

        for (size_t i = 0; i < path.size() - 1; i++) {
            int u = path[i];
            int v = path[i+1];
            for (const auto& e : adj[u]) {
                if (e.v == v) {
                    totalTime += e.time;
                    break; 
                }
            }
        }

        // Print Output
        for (size_t i = 0; i < path.size(); i++) {
            cout << path[i];
            if (i < path.size() - 1) cout << "->";
        }
        cout << " " << totalTime << " " << dist[des] << endl;
    }
}
 int main(){
    long long int n,m,k;
    cin>>k>>n>>m;
    vector<vector<Edge> > adj(n+1); 
    for(long long int i=0; i<m; i++){
        long long int u,v,t,w;
        cin>>u>>v>>t>>w;
        adj[u].push_back(Edge(v,t,w));
        adj[v].push_back(Edge(u,t,w));
    }
     long long int source,destination;
     cin>>source>>destination;
     solve(n,adj,source,destination,k);
    
}