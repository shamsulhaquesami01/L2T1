#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

using namespace std;

// --- DSU & Edge Structures ---
struct Edge {
    int u, v, w;
    // Sort Ascending (Smallest first) for Minimum Spanning Tree
    // This minimizes the bottleneck (Maximum edge on the path)
    bool operator<(const Edge& other) const {
        return w < other.w;
    }
};

struct DSU {
    vector<int> parent;
    DSU(int n) {
        parent.resize(n + 1);
        for(int i = 0; i <= n; i++) parent[i] = i;
    }
    int find(int i) {
        if(parent[i] == i) return i;
        return parent[i] = find(parent[i]);
    }
    void unite(int i, int j) {
        int root_i = find(i);
        int root_j = find(j);
        if(root_i != root_j) parent[root_i] = root_j;
    }
};

// --- Global Graph for the MST ---
// Stores pairs of {neighbor, weight}
vector<pair<int, int>> mst_adj[100005];
bool visited[100005];
int path_bottleneck = -1; // Result variable

// --- DFS to find path in MST ---
// Returns true if target is found in this branch
bool dfs(int u, int target, int current_max) {
    if (u == target) {
        path_bottleneck = current_max;
        return true;
    }
    
    visited[u] = true;
    
    for (auto& edge : mst_adj[u]) {
        int v = edge.first;
        int w = edge.second;
        
        if (!visited[v]) {
            // Recurse: Update current_max to be the max of path so far OR this edge
            if (dfs(v, target, max(current_max, w))) {
                return true;
            }
        }
    }
    return false;
}

void solve() {
    int n, m;
    if (!(cin >> n >> m)) return;

    vector<Edge> edges;
    for (int i = 0; i < m; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        edges.push_back({u, v, w});
    }

    int startNode, endNode;
    cin >> startNode >> endNode;

    // 1. Build MST (Kruskal's)
    sort(edges.begin(), edges.end());
    DSU dsu(n);
    
    // Clear previous MST if running multiple test cases
    for(int i=0; i<=n; i++) {
        mst_adj[i].clear();
        visited[i] = false;
    }

    for (const auto& e : edges) {
        if (dsu.find(e.u) != dsu.find(e.v)) {
            dsu.unite(e.u, e.v);
            // Add edge to the MST adjacency list
            mst_adj[e.u].push_back({e.v, e.w});
            mst_adj[e.v].push_back({e.u, e.w});
        }
    }

    // 2. DFS on the MST to find the unique path
    // We pass '0' as initial max weight because weights are non-negative
    if (dfs(startNode, endNode, 0)) {
        cout << "Mini-Max Bottleneck Weight: " << path_bottleneck << endl;
    } else {
        cout << "No path exists between " << startNode << " and " << endNode << endl;
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    solve();
    return 0;
}