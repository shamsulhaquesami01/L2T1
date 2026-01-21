#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

using namespace std;

// Structure to hold both Graph Edges and Query Edges
struct Edge {
    int u, v;
    int w;
    int id; // id = -1 for Original Graph Edge, id >= 0 for Query Index

    // Sort by weight ascending
    bool operator<(const Edge& other) const {
        return w < other.w;
    }
};

// Standard DSU implementation
struct DSU {
    vector<int> parent;
    vector<int> size;

    DSU(int n) {
        parent.resize(n + 1);
        iota(parent.begin(), parent.end(), 0); // Fill 0, 1, 2...
        size.assign(n + 1, 1);
    }

    int find(int i) {
        if (parent[i] == i)
            return i;
        return parent[i] = find(parent[i]); // Path compression
    }

    void unite(int i, int j) {
        int root_i = find(i);
        int root_j = find(j);
        if (root_i != root_j) {
            // Union by size
            if (size[root_i] < size[root_j])
                swap(root_i, root_j);
            parent[root_j] = root_i;
            size[root_i] += size[root_j];
        }
    }
};

void solve() {
    int n, m;
    if (!(cin >> n >> m)) return;

    vector<Edge> all_edges;
    all_edges.reserve(m + 200000); // Optimization: Reserve memory

    // 1. Read Original Graph Edges
    for (int i = 0; i < m; ++i) {
        int u, v, w;
        cin >> u >> v >> w;
        all_edges.push_back({u, v, w, -1}); // Mark as Original (-1)
    }

    int q;
    cin >> q;
    // 2. Read Query Edges
    for (int i = 0; i < q; ++i) {
        int u, v, w;
        cin >> u >> v >> w;
        all_edges.push_back({u, v, w, i}); // Mark with Query Index (0 to Q-1)
    }

    // 3. Sort ALL edges by weight
    sort(all_edges.begin(), all_edges.end());

    DSU dsu(n);
    vector<string> results(q); // Store answers

    // 4. Process Loop
    for (const auto& edge : all_edges) {
        if (edge.id == -1) {
            // Original Edge: Actually modify the graph structure
            dsu.unite(edge.u, edge.v);
        } else {
            // Query Edge: Just CHECK connectivity
            // If u and v are ALREADY connected, we have a path of strictly smaller edges.
            // This edge would create a cycle and be the heaviest, so NO.
            // Otherwise, YES.
            if (dsu.find(edge.u) != dsu.find(edge.v)) {
                results[edge.id] = "Yes";
            } else {
                results[edge.id] = "No";
            }
        }
    }

    // 5. Output Results
    for (int i = 0; i < q; ++i) {
        cout << results[i] << "\n";
    }
}

int main() {
    // Fast I/O is necessary given constraints
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    solve();
    return 0;
}