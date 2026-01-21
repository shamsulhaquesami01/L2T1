#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

using namespace std;

struct Edge {
    int u, v;
    long long w;
    int id; // 1-based index

    // Sort descending for Maximum Spanning Tree
    bool operator<(const Edge& other) const {
        return w > other.w;
    }
};

struct DSU {
    vector<int> parent;
    DSU(int n) {
        parent.resize(n + 1);
        for(int i = 0; i <= n; i++) parent[i] = i;
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

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, m;
    long long s;
    if (!(cin >> n >> m >> s)) return 0;

    vector<Edge> all_edges;
    for (int i = 0; i < m; i++) {
        int u, v;
        long long w;
        cin >> u >> v >> w;
        all_edges.push_back({u, v, w, i + 1});
    }


    sort(all_edges.begin(), all_edges.end()); // Descending sort (defined in struct)

    DSU dsu(n);
    vector<Edge> edges_to_keep;
    vector<Edge> candidates_for_deletion;

    // Kruskal's for MaxST
    for (const auto& edge : all_edges) {
        if (dsu.find(edge.u) != dsu.find(edge.v)) {
            dsu.unite(edge.u, edge.v);
            edges_to_keep.push_back(edge);
        } else {
            // If it forms a cycle, we don't NEED it for connectivity.
            // It is a candidate for deletion.
            candidates_for_deletion.push_back(edge);
        }
    }
    sort(candidates_for_deletion.begin(), candidates_for_deletion.end(), 
         [](const Edge& a, const Edge& b) {
             return a.w < b.w;
         });

    vector<int> deleted_indices;
    long long current_cost = 0;

    for (const auto& edge : candidates_for_deletion) {
        if (current_cost + edge.w <= s) {
            current_cost += edge.w;
            deleted_indices.push_back(edge.id);
        } else {
            break;
        }
    }

    // Output
    cout << deleted_indices.size() << endl;
    for (size_t i = 0; i < deleted_indices.size(); i++) {
        cout << deleted_indices[i] << (i == deleted_indices.size() - 1 ? "" : " ");
    }
    cout << endl;

    return 0;
}