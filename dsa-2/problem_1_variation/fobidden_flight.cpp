#include <iostream>
#include <vector>
#include <queue>
#include <tuple>

using namespace std;

const long long INF = 1e18;

struct State {
    long long cost;
    int u;
    int used; // 0 or 1 coupon
    bool operator>(const State& other) const { return cost > other.cost; }
};

void solve() {
    int n, m;
    cin >> n >> m;
    
    struct Edge { int v, w; };
    vector<vector<Edge>> adj(n + 1);
    
    // Store all edges to read them
    // Let's assume input format is same, then forbidden edge is given
    struct InputEdge { int u, v, w; };
    vector<InputEdge> all_edges;
    for(int i=0; i<m; ++i) {
        int u, v, w;
        cin >> u >> v >> w;
        all_edges.push_back({u, v, w});
    }

    int forbid_u, forbid_v; 
    cin >> forbid_u >> forbid_v; // The forbidden flight

    // Build graph excluding the forbidden edge
    for(auto& e : all_edges) {
        if(e.u == forbid_u && e.v == forbid_v) continue; // SKIP IT
        adj[e.u].push_back({e.v, e.w});
    }

    // Standard Flight Discount Dijkstra
    vector<vector<long long>> dist(n + 1, vector<long long>(2, INF));
    priority_queue<State, vector<State>, greater<State>> pq;

    dist[1][0] = 0;
    pq.push({0, 1, 0});

    while(!pq.empty()) {
        auto [d, u, state] = pq.top();
        pq.pop();
        if(d > dist[u][state]) continue;

        for(auto& edge : adj[u]) {
            int v = edge.v; 
            int w = edge.w;

            // No coupon
            if(d + w < dist[v][state]) {
                dist[v][state] = d + w;
                pq.push({dist[v][state], v, state});
            }
            // Use coupon
            if(state == 0) {
                if(d + w/2 < dist[v][1]) {
                    dist[v][1] = d + w/2;
                    pq.push({dist[v][1], v, 1});
                }
            }
        }
    }
    cout << dist[n][1] << endl;
}