#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>
#include <climits>
#include <set>
#include <cmath>

using namespace std;

const long long INF = 1e18;

// Edge structure
struct Edge {
    int u, v;
    long long w;
};

void solve() {
    int n, m;
    if (!(cin >> n >> m)) return;

    vector<Edge> edges;
    for (int i = 0; i < m; i++) {
        int u, v;
        long long w;
        cin >> u >> v >> w;
        edges.push_back({u, v, w});
    }

    // 1. Run Bellman-Ford to find a cycle node
    vector<long long> dist(n + 1, 0);
    vector<int> parent(n + 1, -1);
    int cycle_node = -1;

    // Run N times
    for (int i = 0; i < n; i++) {
        cycle_node = -1;
        for (const auto& edge : edges) {
            if (dist[edge.u] + edge.w < dist[edge.v]) {
                dist[edge.v] = dist[edge.u] + edge.w;
                parent[edge.v] = edge.u;
                cycle_node = edge.v;
            }
        }
    }

    if (cycle_node == -1) {
        cout << "No negative cycle exists" << endl;
        return;
    }

    // 2. Cycle Found: Backtrack to ensure we are inside the cycle
    for (int i = 0; i < n; i++) {
        cycle_node = parent[cycle_node];
    }

    // 3. Identify all edges belonging to this cycle
    // We use a set of pairs {u, v} to quickly check which edges are part of the loop
    set<pair<int, int>> cycle_connections;
    int curr = cycle_node;
    do {
        // The edge comes FROM parent[curr] TO curr
        cycle_connections.insert({parent[curr], curr});
        curr = parent[curr];
    } while (curr != cycle_node);

    // 4. Find the edge in the cycle with the Minimum Absolute Weight
    long long min_abs_val = LLONG_MAX;
    Edge edge_to_remove = {-1, -1, 0};

    for (const auto& edge : edges) {
        // Check if this edge connects two nodes in the cycle structure
        if (cycle_connections.count({edge.u, edge.v})) {
            // Logic: Select minimum absolute weight
            if (abs(edge.w) < min_abs_val) {
                min_abs_val = abs(edge.w);
                edge_to_remove = edge;
            }
        }
    }

    // 5. Output
    if (edge_to_remove.u != -1) {
        cout << "Remove edge: " << edge_to_remove.u << " -> " << edge_to_remove.v << endl;
    } else {
        cout << "Could not find cycle edge" << endl;
    }
}

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    solve();
    return 0;
}