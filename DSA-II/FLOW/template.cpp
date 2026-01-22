#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <cstring> // For memset

using namespace std;

const int MAXN = 6; // Maximum number of nodes
const long long INF = 1e18;

// Capacity Matrix: capacity[u][v] stores available capacity from u to v
long long capacity[MAXN][MAXN];
long long actual_capacity[MAXN][MAXN];
// Graph adjacency list (only stores connectivity, not weights)
vector<int> adj[MAXN];
// Parent array to store the path found by BFS
int parent[MAXN];

int n, m;

// BFS to find an augmenting path from s to t
// Returns true if a path exists, false otherwise
bool bfs(int s, int t) {
    // Reset parent array (-1 means not visited)
    memset(parent, -1, sizeof(parent));
    
    queue<int> q;
    q.push(s);
    parent[s] = -2; // Mark source as visited (using a special value like -2)

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (int v : adj[u]) {
            // If v is not visited AND there is residual capacity
            if (parent[v] == -1 && capacity[u][v] > 0) {
                parent[v] = u; // Store where we came from
                // Optimization: If we reached the sink, we can stop immediately
                if (v == t) return true;
                q.push(v);
            }
        }
    }
    return false; // No path found
}

void edmonds_karp(int s, int t, vector< pair<int,int > >& edges) {
    long long max_flow = 0;

    // While there is a path from s to t with available capacity
    while (bfs(s, t)) {
        // 1. Find the bottleneck capacity on this path
        long long path_flow = INF;
        
        // Backtrack from sink (t) to source (s) using parent[]
        for (int v = t; v != s; v = parent[v]) {
            int u = parent[v];
            path_flow = min(path_flow, capacity[u][v]);
        }

        // 2. Update residual capacities
        // Subtract from forward edge, add to backward edge
        for (int v = t; v != s; v = parent[v]) {
            int u = parent[v];
            capacity[u][v] -= path_flow; // Reduce forward capacity
            capacity[v][u] += path_flow; // Increase backward capacity
        }

        // 3. Add to total flow
        max_flow += path_flow;
    }
    cout<<max_flow<<endl;
    for(auto [u,v]:edges){
        cout<<u<<" "<<v<<" "<<actual_capacity[u][v]-capacity[u][v]<<"/"<<actual_capacity[u][v]<<endl;
    }
}

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // Input: Nodes, Edges
    if (!(cin >> n >> m)) return 0;
    vector< pair<int,int > > edges;
    // Reset capacity matrix
    memset(capacity, 0, sizeof(capacity));

    for (int i = 0; i < m; i++) {
        int u, v;
        long long c;
        cin >> u >> v >> c;
        edges.push_back({u,v});
        // Add to adjacency list for BFS traversal
        adj[u].push_back(v);
        adj[v].push_back(u); // Add reverse edge for residual graph connectivity
        
        // Set capacity. 
        // NOTE: If graph is directed, only capacity[u][v] += c.
        // If graph is undirected, usually capacity[u][v] += c AND capacity[v][u] += c.
        // Assuming DIRECTED graph for standard Max Flow problems:
        capacity[u][v] += c; 
        actual_capacity[u][v]+=c;
    }

    int s, t;
    cin >> s >> t; // Source and Sink
    edmonds_karp(s,t,edges);
    return 0;
}