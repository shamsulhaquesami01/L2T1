#include <iostream>
#include <vector>
#include <queue>
#include <tuple>
#include <climits>
#include <algorithm>

using namespace std;

// --- TEMPLATE ---
struct Edge { int to, capacity, flow, rev; };
class MaxFlow {
    int N;
    vector<vector<Edge>> graph;
public:
    MaxFlow(int n) : N(n), graph(n) {}
    int addEdge(int u, int v, int cap) {
        Edge a = {v, cap, 0, (int)graph[v].size()};
        Edge b = {u, 0, 0, (int)graph[u].size()};
        graph[u].push_back(a);
        graph[v].push_back(b);
        return (int)graph[u].size() - 1;
    }
    int bfs(int s, int t, vector<pair<int,int>>& parent) {
        fill(parent.begin(), parent.end(), make_pair(-1, -1));
        queue<int> q;
        q.push(s); parent[s] = {s, -1};
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (int i = 0; i < graph[u].size(); i++) {
                Edge &e = graph[u][i];
                if (parent[e.to].first == -1 && e.capacity > e.flow) {
                    parent[e.to] = {u, i};
                    if (e.to == t) return 1;
                    q.push(e.to);
                }
            }
        }
        return 0;
    }
    int edmondsKarp(int s, int t) {
        int maxFlow = 0;
        vector<pair<int,int>> parent(N);
        while (bfs(s, t, parent)) {
            int pathFlow = INT_MAX;
            for (int v = t; v != s; ) {
                auto [u, idx] = parent[v];
                pathFlow = min(pathFlow, graph[u][idx].capacity - graph[u][idx].flow);
                v = u;
            }
            for (int v = t; v != s; ) {
                auto [u, idx] = parent[v];
                graph[u][idx].flow += pathFlow;
                int rev_idx = graph[u][idx].rev;
                graph[v][rev_idx].flow -= pathFlow;
                v = u;
            }
            maxFlow += pathFlow;
        }
        return maxFlow;
    }
};

double dist(pair<int,int> a, pair<int,int> b) {
    return sqrt(pow(a.first - b.first, 2) + pow(a.second - b.second, 2));
}

void solve(int t) {
    int N;
    double D;
    cin >> N >> D;
    
    vector<tuple<int, int, int, int>> floes(N);
    int total_penguins = 0;
    
    for(int i=0; i<N; i++) {
        cin >> get<0>(floes[i]) >> get<1>(floes[i]) >> get<2>(floes[i]) >> get<3>(floes[i]);
        total_penguins += get<2>(floes[i]);
    }

    cout << "Case " << t << ":";
    bool possible = false;

    // Try every floe 'target' as the meeting point
    for(int target = 0; target < N; target++) {
        // Build graph from scratch for each target
        // Nodes: S(0), T(2N+1). Floe i: in(i+1), out(i+1+N)
        int S = 0, T = 2 * N + 1;
        MaxFlow mf(T + 1);

        for(int i=0; i<N; i++) {
            auto [x, y, n_pen, limit] = floes[i];
            
            // Source to In-Node (Initial penguins)
            mf.addEdge(S, i+1, n_pen);
            
            // In-Node to Out-Node (Jump limit)
            mf.addEdge(i+1, i+1+N, limit);
            
            // Out-Node to other In-Nodes (Jumps)
            for(int j=0; j<N; j++) {
                if(i == j) continue;
                auto [x2, y2, n2, l2] = floes[j];
                if(dist({x, y}, {x2, y2}) <= D + 1e-9) {
                    mf.addEdge(i+1+N, j+1, INT_MAX);
                }
            }
        }
        
        // Connect target's In-Node to Sink (Infinite capacity to accept penguins)
        mf.addEdge(target+1, T, INT_MAX); 

        if(mf.edmondsKarp(S, T) == total_penguins) {
            cout << " " << target;
            possible = true;
        }
    }
    
    if(!possible) cout << " -1";
    cout << endl;
}

int main() {
    int t;
    cin >> t;
    for (int i = 1; i <= t; i++) {
        solve(i);
    }
}