#include <iostream>
#include <vector>
#include <queue>
#include <tuple>
#include <climits>
#include <algorithm>

using namespace std;

// --- YOUR STANDARD TEMPLATE START ---
struct Edge {
    int to;
    int capacity;
    int flow;
    int rev;
};

class MaxFlow {
public:
    int N;
    vector<vector<Edge>> graph;

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
        q.push(s);
        parent[s] = {s, -1};

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
// --- TEMPLATE END ---

void solve() {
    int N;
    if (!(cin >> N)) return;

    // Reading costs
    vector<int> M_cost(N);
    vector<int> V_cost(N);
    for(int i = 0; i < N; i++) cin >> M_cost[i];
    for(int i = 0; i < N; i++) cin >> V_cost[i];

    // Nodes: S=0, People=1..N, T=N+1
    int S = 0;
    int T = N + 1;
    MaxFlow mf(T + 1);

    // 1. Build Source/Sink connections
    for(int i = 0; i < N; i++) {
        // Edge S -> i with Capacity V_cost[i]
        // (Cutting this means i is disconnected from Source, so i picks VinaGone, pay V_cost)
        mf.addEdge(S, i + 1, V_cost[i]);

        // Edge i -> T with Capacity M_cost[i]
        // (Cutting this means i is disconnected from Sink, so i picks MobiZone, pay M_cost)
        mf.addEdge(i + 1, T, M_cost[i]);
    }

    // 2. Build Penalty connections (C_ij)
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            int c;
            cin >> c;
            // Since matrix is symmetric and Cii = 0, we can just add edges for i < j
            // Or add all, capacity doesn't double count if we cut correctly.
            // Usually, we add undirected edge capacity C.
            // In MaxFlow graph, undirected capacity C is represented as:
            // i -> j cap C AND j -> i cap C.
            if(i < j && c > 0) {
                mf.addEdge(i + 1, j + 1, c);
                mf.addEdge(j + 1, i + 1, c);
            }
        }
    }

    // Min Cost = Max Flow
    cout << mf.edmondsKarp(S, T) << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    solve();
    return 0;
}