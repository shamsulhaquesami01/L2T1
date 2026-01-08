#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

typedef long long ll;

struct Edge {
    int to;
    int weight;
};

// State for Priority Queue: {distance, vertex}
// Using greater<> for Min-Heap
typedef pair<ll, int> State;

void solve() {
    int n, m, k;
    if (!(cin >> n >> m >> k)) return;

    // Adjacency List
    vector<vector<Edge>> adj(n + 1);
    for (int i = 0; i < m; ++i) {
        int u, v, w;
        cin >> u >> v >> w;
        adj[u].push_back({v, w});
    }

    // To store the Top K shortest distances for every node.
    // We use a priority_queue (Max-Heap) for each node to easily 
    // keep track of the "worst of the best" so we can discard it if a better path comes.
    vector<priority_queue<ll>> best_dists(n + 1);

    // Main Dijkstra Priority Queue (Min-Heap)
    priority_queue<State, vector<State>, greater<State>> pq;

    // Initial State: Distance to start node (1) is 0
    pq.push({0, 1});
    best_dists[1].push(0);

    while (!pq.empty()) {
        ll d = pq.top().first;
        int u = pq.top().second;
        pq.pop();

        // Optimization: If this path 'd' is already worse than the K-th best path 
        // we have found for 'u', there is no point processing it.
        // (This check handles "stale" entries in the main PQ)
        if (d > best_dists[u].top()) continue;

        for (const auto& edge : adj[u]) {
            int v = edge.to;
            int w = edge.weight;
            ll new_d = d + w;

            // Logic to maintain only K best paths for node v
            if (best_dists[v].size() < k) {
                // If we haven't found k paths yet, just add it
                best_dists[v].push(new_d);
                pq.push({new_d, v});
            } else if (new_d < best_dists[v].top()) {
                // If we found a path shorter than the current K-th shortest,
                // remove the K-th shortest (the largest in the max-heap) and add the new one.
                best_dists[v].pop();
                best_dists[v].push(new_d);
                pq.push({new_d, v});
            }
        }
    }

    // The best_dists[n] contains the K shortest paths, but since it's a Max-Heap,
    // they are in descending order (largest at top). We need to sort them.
    vector<ll> result;
    while (!best_dists[n].empty()) {
        result.push_back(best_dists[n].top());
        best_dists[n].pop();
    }
    
    // Sort to print in increasing order
    sort(result.begin(), result.end());

    for (int i = 0; i < k; ++i) {
        cout << result[i] << (i == k - 1 ? "" : " ");
    }
    cout << endl;
}

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    solve();
    return 0;
}