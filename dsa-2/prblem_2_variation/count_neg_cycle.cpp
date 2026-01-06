#include <iostream>
#include <vector>
#include <tuple>
#include <set>
#include <algorithm>

using namespace std;

// Standard Edge structure
struct Edge {
    int u, v;
    long long w;
};

void solve() {
    int n, m;
    if (!(cin >> n >> m)) return;

    vector<Edge> edges;
    // We don't strictly need adjacency list for Bellman-Ford, 
    // but the original code built it. We'll stick to the Edge list for cleaner BF.
    for (int i = 0; i < m; i++) {
        int u, v;
        long long w;
        cin >> u >> v >> w;
        edges.push_back({u, v, w});
    }

    // Initialization
    // dist initialized to 0 ensures we can detect cycles even if the graph is disconnected
    vector<long long> dist(n + 1, 0);
    vector<int> parent(n + 1, -1);
    
    // Set to store nodes that are part of (or reachable from) negative cycles
    set<int> cycle_nodes;

    // Bellman-Ford Relaxation
    // Logic: Run N times. 
    // If relaxation happens in the N-th iteration (index n-1), record the node.
    for (int i = 0; i < n; i++) {
        for (const auto& edge : edges) {
            if (dist[edge.u] + edge.w < dist[edge.v]) {
                dist[edge.v] = dist[edge.u] + edge.w;
                parent[edge.v] = edge.u;
                
                // If this is the Nth iteration (last one), capture the node
                if (i == n - 1) {
                    cycle_nodes.insert(edge.v);
                }
            }
        }
    }

    if (cycle_nodes.empty()) {
        cout << "0" << endl;
    } else {
        // Logic from your code: Find all nodes involved in these cycles
        set<int> all_cycle_nodes;
        
        for (int node : cycle_nodes) {
            int curr = node;
            set<int> path;
            
            // Backtrack N times to ensure we are deep inside the cycle
            // (or trace the path as your original code did)
            for (int i = 0; i < n; i++) {
                if (curr == -1) break; // Safety check
                curr = parent[curr];
                path.insert(curr);
            }
            all_cycle_nodes.insert(path.begin(), path.end());
        }

        // Logic Preservation: Your code output the size of the initial set 'cycle_nodes'
        // which represents the number of nodes relaxed in the final step.
        cout << "At least " << cycle_nodes.size() << " negative cycle(s) detected" << endl;
    }
}

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    solve();
    return 0;
}