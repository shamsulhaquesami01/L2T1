#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// Structure to represent an edge
struct Edge {
    int u, v;
    int weight;

    // Comparator for sorting edges by weight
    bool operator<(const Edge& other) const {
        return weight < other.weight;
    }
};

// Disjoint Set Union (DSU) structure for efficient cycle detection
struct DSU {
    vector<int> parent;
    
    DSU(int n) {
        parent.resize(n);
        for (int i = 0; i < n; i++) {
            parent[i] = i;
        }
    }

    // Find the representative of the set containing x with path compression
    int find(int x) {
        if (parent[x] == x) return x;
        return parent[x] = find(parent[x]);
    }

    // Unite the sets containing x and y. Returns true if successful, 
    // false if they were already in the same set.
    bool unite(int x, int y) {
        int rootX = find(x);
        int rootY = find(y);
        if (rootX != rootY) {
            parent[rootX] = rootY;
            return true;
        }
        return false;
    }
};

void solve() {
    int n, m;
    // Read input until n=0, m=0
    while (cin >> n >> m && (n != 0 || m != 0)) {
        vector<Edge> edges(m);
        for (int i = 0; i < m; i++) {
            cin >> edges[i].u >> edges[i].v >> edges[i].weight;
        }

        // If there are no edges, it's immediately a forest
        if (m == 0) {
            cout << "forest" << endl;
            continue;
        }

        // Step 1: Sort edges by weight (Kruskal's logic)
        sort(edges.begin(), edges.end());

        DSU dsu(n);
        vector<int> heavy_cycle_edges;

        // Step 2: Iterate through edges
        for (const auto& edge : edges) {
            // Try to unite the sets
            if (!dsu.unite(edge.u, edge.v)) {
                // If unite returns false, u and v are already connected.
                // This edge creates a cycle. Since edges are sorted, 
                // this is the heaviest edge in that cycle.
                heavy_cycle_edges.push_back(edge.weight);
            }
        }

        // Step 3: Output results
        if (heavy_cycle_edges.empty()) {
            cout << "forest" << endl;
        } else {
            // Edges are processed in increasing order, so the result is already sorted
            for (size_t i = 0; i < heavy_cycle_edges.size(); i++) {
                cout << heavy_cycle_edges[i] << (i == heavy_cycle_edges.size() - 1 ? "" : " ");
            }
            cout << endl;
        }
    }
}

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    solve();
    return 0;
}