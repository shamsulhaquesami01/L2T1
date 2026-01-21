#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

using namespace std;

// Structure to represent a road
struct Edge {
    int u, v;
    long long w;
};

// Comparator to sort edges by speed limit
bool compareEdges(const Edge& a, const Edge& b) {
    return a.w < b.w;
}

// Disjoint Set Union (DSU) structure for efficient connectivity checks
struct DSU {
    vector<int> parent;
    int components; // To track the number of connected components
    
    DSU(int n) {
        parent.resize(n + 1);
        iota(parent.begin(), parent.end(), 0);
        components = n;
    }
    
    int find(int i) {
        if (parent[i] == i)
            return i;
        return parent[i] = find(parent[i]);
    }
    
    bool unite(int i, int j) {
        int root_i = find(i);
        int root_j = find(j);
        if (root_i != root_j) {
            parent[root_i] = root_j;
            components--;
            return true;
        }
        return false;
    }
};

void solve() {
    int n, m;
    long long k;
    if (!(cin >> n >> m >> k)) return;
    
    vector<Edge> edges(m);
    for (int i = 0; i < m; ++i) {
        cin >> edges[i].u >> edges[i].v >> edges[i].w;
    }
    
    // Sort edges by weight to apply Kruskal's logic
    sort(edges.begin(), edges.end(), compareEdges);
    
    DSU dsu_small(n); // Manages connectivity using only edges < k
    DSU dsu_all(n);   // Manages connectivity for the standard MST approach
    
    long long max_lt_k = -1;
    long long min_ge_k = -1;
    long long cost_all = 0;
    
    for (const auto& edge : edges) {
        if (edge.w < k) {
            // Logic for edges < k
            dsu_small.unite(edge.u, edge.v);
            max_lt_k = edge.w; // Since sorted, this updates to the largest seen so far
            
            // For the general MST, these edges have cost 0
            dsu_all.unite(edge.u, edge.v);
        } else {
            // Logic for edges >= k
            if (min_ge_k == -1) {
                min_ge_k = edge.w; // First edge >= k is the smallest
            }
            
            // For general MST, add cost (w - k) if this edge connects new components
            if (dsu_all.unite(edge.u, edge.v)) {
                cost_all += (edge.w - k);
            }
        }
    }
    
    if (dsu_small.components == 1) {
        // Case 1: Graph is connected using only edges < k
        // Option A: Use only edges < k, bump the largest one up to k
        long long ans = k - max_lt_k;
        
        // Option B: Use edges < k but swap one with an edge >= k (reduce it to k)
        if (min_ge_k != -1) {
            ans = min(ans, min_ge_k - k);
        }
        cout << ans << endl;
    } else {
        // Case 2: Graph needs edges >= k to connect
        // The standard MST cost calculated is the answer
        cout << cost_all << endl;
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int t;
    if (cin >> t) {
        while (t--) {
            solve();
        }
    }
    return 0;
}