#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <climits>

using namespace std;

const long long INF = 1e18;

// Edge structure
struct Edge {
    int v;
    long long w;
};

// State structure for Priority Queue
struct State {
    long long cost;
    int u;
    int used; // 0 = false, 1 = true

    bool operator>(const State& other) const {
        return cost > other.cost;
    }
};

// Structure to track the path
struct ParentInfo {
    int pNode;  // Previous Node
    int pState; // Previous State (0 or 1)
};

void solve() {
    int n, m;
    if (!(cin >> n >> m)) return;

    vector<vector<Edge>> adj(n + 1);
    for (int i = 0; i < m; i++) {
        int u, v;
        long long w;
        cin >> u >> v >> w;
        adj[u].push_back({v, w});
    }

    // dist[node][state]
    vector<vector<long long>> dist(n + 1, vector<long long>(2, INF));
    
    // parent[node][state] to reconstruct path
    // Initialize with {-1, -1} to mark start
    vector<vector<ParentInfo>> parent(n + 1, vector<ParentInfo>(2, {-1, -1}));

    priority_queue<State, vector<State>, greater<State>> pq;

    // Start: Node 1, Cost 0, Coupon 0
    dist[1][0] = 0;
    pq.push({0, 1, 0});

    while(!pq.empty()) {
        auto [d, u, used] = pq.top();
        pq.pop();

        if (d > dist[u][used]) continue;

        for (const auto& edge : adj[u]) {
            int v = edge.v;
            long long w = edge.w;

            // --- OPTION 1: Normal Travel (Keep same state) ---
            if (d + w < dist[v][used]) {
                dist[v][used] = d + w;
                parent[v][used] = {u, used}; // Came from u, same state
                pq.push({dist[v][used], v, used});
            }

            // --- OPTION 2: Use Coupon (0 -> 1) ---
            if (used == 0) {
                long long newCost = d + (w / 2);
                if (newCost < dist[v][1]) {
                    dist[v][1] = newCost;
                    parent[v][1] = {u, 0}; // Came from u, was in state 0
                    pq.push({dist[v][1], v, 1});
                }
            }
        }
    }

    // --- Path Reconstruction ---
    
    // 1. Determine which end state is better (usually state 1, but check both)
    int currNode = n;
    int currState = 1;
    if (dist[n][0] < dist[n][1]) {
        currState = 0;
    }

    // If unreachable
    if (dist[n][currState] == INF) {
        cout << "No path found" << endl;
        return;
    }

    cout << "Minimum Cost: " << dist[n][currState] << endl;

    // 2. Backtrack using parent array
    vector<int> path;
    while (currNode != -1) {
        path.push_back(currNode);
        ParentInfo p = parent[currNode][currState];
        currNode = p.pNode;
        currState = p.pState;
    }

    // 3. Print Path (Reverse it first)
    reverse(path.begin(), path.end());
    cout << "Path: ";
    for (size_t i = 0; i < path.size(); i++) {
        cout << path[i] << (i == path.size() - 1 ? "" : " -> ");
    }
    cout << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    solve();
    return 0;
}