#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>

using namespace std;

struct Edge {
    int u, v, w;
    int id; // Optional: Unique ID to help identify precise edge to delete
};

// Returns true if a cycle was found and removed, false otherwise
bool detectAndRemoveCycle(int N, vector<Edge>& edges) {
    vector<long long> dist(N + 1, 0);
    vector<int> parent(N + 1, -1);
    int cycle_node = -1;

    // 1. Run Bellman-Ford Relaxation
    for (int i = 0; i < N; ++i) {
        cycle_node = -1;
        for (const auto& edge : edges) {
            if (dist[edge.u] + edge.w < dist[edge.v]) {
                dist[edge.v] = dist[edge.u] + edge.w;
                parent[edge.v] = edge.u;
                cycle_node = edge.v;
            }
        }
    }

    // 2. If no cycle detected, return false
    if (cycle_node == -1) {
        cout << "No negative cycle found." << endl;
        return false; 
    }

    cout << "Negative cycle detected! Removing it..." << endl;

    // 3. Step back N times to ensure we are inside the loop (remove tail)
    for (int i = 0; i < N; ++i) {
        cycle_node = parent[cycle_node];
    }

    // 4. Trace the actual cycle
    vector<int> cycle;
    int curr = cycle_node;
    while (true) {
        cycle.push_back(curr);
        if (curr == cycle_node && cycle.size() > 1) break;
        curr = parent[curr];
    }
    
    // Reverse to get correct direction: u -> v -> ...
    reverse(cycle.begin(), cycle.end());

    // 5. Select an edge to remove. 
    // We will remove the edge connecting the last node back to the first node.
    // Cycle vector looks like: [A, B, C, A]
    // We want to remove the edge C -> A.
    int u_to_remove = cycle[cycle.size() - 2]; 
    int v_to_remove = cycle[cycle.size() - 1];

    cout << "Cycle path identified: ";
    for (int node : cycle) cout << node << " ";
    cout << endl;

    // 6. Find and Erase the edge (u_to_remove -> v_to_remove) from the list
    for (auto it = edges.begin(); it != edges.end(); ++it) {
        if (it->u == u_to_remove && it->v == v_to_remove) {
            cout << "Removing edge: " << it->u << " -> " << it->v << " (Weight: " << it->w << ")" << endl;
            edges.erase(it); // Deletes the edge
            return true;
        }
    }

    return true;
}

int main() {
    int n, m;
    if (cin >> n >> m) {
        vector<Edge> edges;
        for (int i = 0; i < m; i++) {
            int u, v, w;
            cin >> u >> v >> w;
            edges.push_back({u, v, w, i});
        }

        // Run the function
        detectAndRemoveCycle(n, edges);

        // Optional: Verify if it's gone by running detection again
        cout << "\n--- Verification Run ---" << endl;
        detectAndRemoveCycle(n, edges);
    }
    return 0;
}
