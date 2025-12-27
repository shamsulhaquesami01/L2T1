#include <iostream>
#include <vector>
#include <climits> 

using namespace std;

class Edge {
    public:
    int u, v, w;
    Edge(int u, int v, int w){
        this->u=u;
        this->v=v;
        this->w=w;
    }
};
void bellmanFord(vector<int>& source, int N, int E, vector<Edge>& edges, vector<int>&dest) {
    // Initialize distances
    vector<int> dist(N+1, INT_MAX);
    for(int i:source) dist[i]=0;

    // Relaxation Step (Run N-1 times)
    for (int i = 1; i <= N - 1; i++) {
        // 2. Use 'const auto&' for efficiency
        for (const auto& edge : edges) {
            int u = edge.u;
            int v = edge.v;
            int w = edge.w;

            // Check for INT_MAX to avoid overflow
            if (dist[u] != INT_MAX && dist[u] + w < dist[v]) {
                
                dist[v] = dist[u] + w; 
            }
        }
    }

    // Negative Cycle Check
    for (const auto& edge : edges) {
        int u = edge.u;
        int v = edge.v;
        int w = edge.w;
        if (dist[u] != INT_MAX && dist[u] + w < dist[v]) {
            cout << "Abyss detected!" << endl;
            return;
        }
    }
    for(int i:dest){
        if(dist[i]== INT_MAX) {
            cout<<"INF"<<endl;
        }
        else cout<<dist[i]<<endl;
    }
}
int main() {
    int n,m,k,b,e;
    cin>>n>>m>>k>>b>>e;
    vector<int>source;
    for(int i=0; i<k; i++){
         int a;cin>>a;
         source.push_back(a);
    }
    vector<int>block;
    for(int i=0; i<b; i++){
         int a;cin>>a;
         block.push_back(a);
    }
     vector<int>dest;
    for(int i=0; i<e; i++){
         int a;cin>>a;
         dest.push_back(a);
    }
    vector<Edge> edges;
    for(int i=0; i<m;i++){
        int u,v,w;
        cin>>u>>v>>w;
        bool flag= true;
        for(int x:block){
            if(u==x || v==x) flag = false;
        }
        if(flag) edges.push_back(Edge(u,v,w));
        
    }
    bellmanFord(source, n, m, edges,dest);
    return 0;
}