/*
Given an undirected connected graph represented using an adjacency list adj[][][]. For each vertex i, the list adj[i] contains entries of the form {u, w1, w2}, where u is the neighboring vertex, w1 is the weight of the straight edge between i and u, and w2 is the weight of the curved edge between them. Thus, every pair of connected vertices has two parallel edges: one straight and one curved.
We are also given two vertices a and b, and we need to determine the minimum cost required to travel from a to b under the constraint that:

we may use any number of straight edges
we may use at most one curved edge in the entire path.
If no such path exists that satisfies this restriction, we must return -1.

*/

#include <iostream>
#include <vector>
#include <queue>
#include <tuple>
#include <climits>
#include <algorithm>

using namespace std;

const long long INF = 1e18;

struct State {
    long long cost;
    int u;
    int curvedUsed; // 0 = false, 1 = true

    // Operator overloading for Min-Priority Queue (smallest cost at top)
    bool operator>(const State& other) const {
        return cost > other.cost;
    }
};

long long minCostWithAtMostOneCurved(int n, vector<vector<vector<int>>>& adj, int src, int dest) {
    // dist[i][0] stores min cost to reach node i with 0 curved edges
    // dist[i][1] stores min cost to reach node i with 1 curved edge
    // Use n + 1 to handle 1-based indexing safely if needed
    vector<vector<long long>> dist(n + 1, vector<long long>(2, INF));
    
    // Priority Queue stores {cost, vertex, curvedUsed}
    priority_queue<State, vector<State>, greater<State>> pq;

    // Initialization
    dist[src][0] = 0;
    // Note: dist[src][1] remains INF because we start with 0 curved edges used
    pq.push({0, src, 0});

    while (!pq.empty()) {
        auto [d, u, state] = pq.top();
        pq.pop();

        // If we found a shorter path to this state already, skip
        if (d > dist[u][state]) continue;

        // Explore neighbors
        for (auto& edge : adj[u]) {
            int v = edge[0];
            int w1 = edge[1]; // Straight edge weight
            int w2 = edge[2]; // Curved edge weight

            // --- TRANSITION 1: Take Straight Edge ---
            // Valid for both state 0 (remain 0) and state 1 (remain 1)
            if (dist[u][state] + w1 < dist[v][state]) {
                dist[v][state] = dist[u][state] + w1;
                pq.push({dist[v][state], v, state});
            }

            // --- TRANSITION 2: Take Curved Edge ---
            // Only valid if we are currently in state 0
            if (state == 0) {
                if (dist[u][0] + w2 < dist[v][1]) {
                    dist[v][1] = dist[u][0] + w2;
                    pq.push({dist[v][1], v, 1});
                }
            }
        }
    }

    // The answer is the minimum of reaching 'dest' with 0 curved edges 
    // OR reaching 'dest' with 1 curved edge.
    long long ans = min(dist[dest][0], dist[dest][1]);

    if (ans == INF) return -1;
    return ans;
}

// Example driver code (Optional, for testing)
int main() {
    int n = 3;
    // Adjacency list: adj[u] = list of {v, w1, w2}
    // Let's say: 
    // 0 -> 1 (str: 5, crv: 2)
    // 1 -> 2 (str: 5, crv: 100)
    // Path 0->1(curve)->2(straight) = 2 + 5 = 7
    // Path 0->1(straight)->2(straight) = 5 + 5 = 10
    
    vector<vector<vector<int>>> adj(n);
    
    // Edge 0-1
    adj[0].push_back({1, 5, 2});
    adj[1].push_back({0, 5, 2});

    // Edge 1-2
    adj[1].push_back({2, 5, 100});
    adj[2].push_back({1, 5, 100});

    cout << minCostWithAtMostOneCurved(n, adj, 0, 2) << endl; // Output should be 7

    return 0;
}
