#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <climits>

using namespace std;

const long long INF = 1e18;

struct Edge {
    int v;
    long long w;
};

struct State {
    long long cost;
    int u;
    int used;
    int steps;
    bool operator>(const State& other) const { return cost > other.cost; }
};

void solve() {
    int n, m;
    if (!(cin >> n >> m)) return;

    // Use REVERSED Adjacency List
    vector<vector<Edge>> rev_adj(n + 1);
    for (int i = 0; i < m; i++) {
        int u, v; 
        long long w;
        cin >> u >> v >> w;
        // Store as v -> u (Reverse direction)
        rev_adj[v].push_back({u, w}); 
    }

    // dist[node][used][steps]
    vector<vector<vector<long long>>> dist(n + 1, vector<vector<long long>>(2, vector<long long>(7, INF)));
    priority_queue<State, vector<State>, greater<State>> pq;

    // Start at Node N (Destination), Step 0
    dist[n][0][0] = 0;
    pq.push({0, n, 0, 0});

    while (!pq.empty()) {
        auto [d, u, used, steps] = pq.top();
        pq.pop();

        int current_step_idx = min(steps, 6);
        if (d > dist[u][used][current_step_idx]) continue;

        for (const auto& edge : rev_adj[u]) {
            int v = edge.v; // This is actually going 'backwards' towards source
            long long w = edge.w;
            int next_steps = steps + 1;
            int next_step_idx = min(next_steps, 6);

            // Option 1: Normal
            if (d + w < dist[v][used][next_step_idx]) {
                dist[v][used][next_step_idx] = d + w;
                pq.push({dist[v][used][next_step_idx], v, used, next_steps});
            }

            // Option 2: Use Coupon
            // Constraint: Only for first 5 steps of the REVERSED path 
            // (which are the LAST 5 of the actual path)
            if (used == 0 && steps < 5) {
                long long newCost = d + (w / 2);
                if (newCost < dist[v][1][next_step_idx]) {
                    dist[v][1][next_step_idx] = newCost;
                    pq.push({dist[v][1][next_step_idx], v, 1, next_steps});
                }
            }
        }
    }

    // Answer is shortest path to Node 1 (Source)
    long long ans = INF;
    for (int i = 0; i <= 6; i++) {
        ans = min(ans, min(dist[1][0][i], dist[1][1][i]));
    }
    cout << ans << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    solve();
    return 0;
}