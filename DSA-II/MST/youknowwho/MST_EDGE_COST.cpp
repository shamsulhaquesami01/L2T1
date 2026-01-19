//same thing , ebar just protteker cost tai dekhaite bolse

#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

using namespace std;

const int MAXN = 100005;
const int LOGN = 20; // 2^20 > 100000

struct Edge {
    int u, v, id;
    long long w;
    // Sorting for Kruskal's
    bool operator<(const Edge& other) const {
        return w < other.w;
    }
};

struct DSU {
    vector<int> parent;
    DSU(int n) {
        parent.resize(n + 1);
        for (int i = 0; i <= n; i++) parent[i] = i;
    }
    int find(int i) {
        if (parent[i] == i) return i;
        return parent[i] = find(parent[i]);
    }
    void unite(int i, int j) {
        int root_i = find(i);
        int root_j = find(j);
        if (root_i != root_j) parent[root_i] = root_j;
    }
};

// MST Adjacency List: pair<neighbor, weight>
vector<pair<int, int>> mst_adj[MAXN];

// Binary Lifting Tables
int up[MAXN][LOGN];
int max_edge[MAXN][LOGN];
int depth[MAXN];
bool in_mst[200005]; // Tracks if edge ID is in MST

// DFS to build Binary Lifting Table
void dfs(int u, int p, int w, int d) {
    depth[u] = d;
    up[u][0] = p;
    max_edge[u][0] = w;

    // Fill tables for 2^1, 2^2, ...
    for (int i = 1; i < LOGN; i++) {
        up[u][i] = up[up[u][i - 1]][i - 1];
        max_edge[u][i] = max(max_edge[u][i - 1], max_edge[up[u][i - 1]][i - 1]);
    }

    for (auto& edge : mst_adj[u]) {
        int v = edge.first;
        int weight = edge.second;
        if (v != p) {
            dfs(v, u, weight, d + 1);
        }
    }
}

// Function to find max weight on path between u and v
int getMaxOnPath(int u, int v) {
    if (depth[u] < depth[v]) swap(u, v);

    int max_w = 0;

    // 1. Lift u to same depth as v
    for (int i = LOGN - 1; i >= 0; i--) {
        if (depth[u] - (1 << i) >= depth[v]) {
            max_w = max(max_w, max_edge[u][i]);
            u = up[u][i];
        }
    }

    if (u == v) return max_w;

    // 2. Lift both to just below LCA
    for (int i = LOGN - 1; i >= 0; i--) {
        if (up[u][i] != up[v][i]) {
            max_w = max(max_w, max_edge[u][i]);
            max_w = max(max_w, max_edge[v][i]);
            u = up[u][i];
            v = up[v][i];
        }
    }

    // 3. Account for the edges connecting to LCA
    max_w = max(max_w, max_edge[u][0]);
    max_w = max(max_w, max_edge[v][0]);

    return max_w;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    if (!(cin >> n >> m)) return 0;

    vector<Edge> edges;
    vector<Edge> original_edges; // To keep input order

    for (int i = 0; i < m; i++) {
        int u, v;
        long long w;
        cin >> u >> v >> w;
        edges.push_back({u, v, i, w});
        original_edges.push_back({u, v, i, w});
    }

    // 1. Kruskal's Algorithm
    sort(edges.begin(), edges.end());
    DSU dsu(n);
    long long mst_weight = 0;

    for (const auto& e : edges) {
        if (dsu.find(e.u) != dsu.find(e.v)) {
            dsu.unite(e.u, e.v);
            mst_weight += e.w;
            in_mst[e.id] = true; // Mark edge as part of MST
            
            // Build MST Graph
            mst_adj[e.u].push_back({e.v, (int)e.w});
            mst_adj[e.v].push_back({e.u, (int)e.w});
        }
    }

    // 2. Precompute LCA & Max Edge Weights
    // Start DFS from node 1 (assuming graph is connected per problem statement)
    // Root has parent 1 and edge weight 0 to parent
    dfs(1, 1, 0, 0);

    // 3. Answer Queries
    for (const auto& e : original_edges) {
        if (in_mst[e.id]) {
            // Case 1: Edge is already in MST
            cout << mst_weight << "\n";
        } else {
            // Case 2: Edge not in MST. Force it in.
            // Cost = MST_Weight + New_Edge - Max_Edge_On_Cycle
            int max_on_cycle = getMaxOnPath(e.u, e.v);
            cout << mst_weight + e.w - max_on_cycle << "\n";
        }
    }

    return 0;
}