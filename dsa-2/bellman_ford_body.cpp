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

void bellmanFord(int startNode, int N, int E, vector<Edge>& edges) {
    // Initialize distances
    vector<int> dist(N, INT_MAX);
    dist[startNode] = 0;

    // Relaxation Step (Run N-1 times)
    for (int i = 0; i < N - 1; i++) {
        // 2. Use 'const auto&' for efficiency
        for (const auto& edge : edges) {
            int u = edge.u;
            int v = edge.v;
            int w = edge.w;

            // Check for INT_MAX to avoid overflow
            if (dist[u] != INT_MAX && dist[u] + w < dist[v]) {
                // 3. FIXED TYPO: Use '=' for assignment, not '<'
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
            cout << "Negative cycle detected!" << endl;
            return;
        }
    }

    // Print result
    for(int i = 0; i < N; i++) {
        cout << i << " : " << dist[i] << endl;
    }
}

int main() {
    int n,m;
    cin>>n,m;
    vector<Edge> edges;
    for(int i=0; i<m;i++){
        int u,v,w;
        cin>>u>>v>>w;
        edges.push_back(Edge(u,v,w));
    }

    bellmanFord(0, n, m, edges);
    return 0;
}