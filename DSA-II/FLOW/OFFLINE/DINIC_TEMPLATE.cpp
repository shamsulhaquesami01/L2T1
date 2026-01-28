#include <vector>
#include <queue>
#include <algorithm>
#include <climits>
#include <iostream>

using namespace std;

struct Edge {
    int to;
    long long capacity;
    long long flow;
    int rev; // Index of reverse edge
};

class Dinic {
public: // Made public to access graph for output printing
    int N;
    vector<vector<Edge>> graph;
    vector<int> level; // Level graph (distance from source)
    vector<int> ptr;   // Next edge index to explore (optimization)

    Dinic(int n) : N(n), graph(n), level(n), ptr(n) {}

    // Modified to return the index of the added edge
    int addEdge(int u, int v, long long cap) {
        Edge a = {v, cap, 0, (int)graph[v].size()};
        Edge b = {u, 0, 0, (int)graph[u].size()}; // Reverse edge with 0 capacity
        graph[u].push_back(a);
        graph[v].push_back(b);
        return (int)graph[u].size() - 1; // Return index of forward edge
    }

    // 1. BFS to build the Level Graph
    bool bfs(int s, int t) {
        fill(level.begin(), level.end(), -1);
        level[s] = 0;
        queue<int> q;
        q.push(s);

        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (const auto &e : graph[u]) {
                if (e.capacity - e.flow > 0 && level[e.to] == -1) {
                    level[e.to] = level[u] + 1;
                    q.push(e.to);
                }
            }
        }
        return level[t] != -1;
    }

    // 2. DFS to find blocking flow
    long long dfs(int u, int t, long long pushed) {
        if (pushed == 0 || u == t) return pushed;

        for (int &cid = ptr[u]; cid < graph[u].size(); cid++) {
            auto &e = graph[u][cid];
            if (level[u] + 1 != level[e.to] || e.capacity - e.flow == 0) continue;

            long long tr = dfs(e.to, t, min(pushed, e.capacity - e.flow));
            if (tr == 0) continue;

            e.flow += tr;
            graph[e.to][e.rev].flow -= tr;
            return tr;
        }
        return 0;
    }

    long long max_flow(int s, int t) {
        long long flow = 0;
        while (bfs(s, t)) {
            fill(ptr.begin(), ptr.end(), 0);
            while (long long pushed = dfs(s, t, 1e18)) {
                flow += pushed;
            }
        }
        return flow;
    }
};

struct InputEdge {
    int u, v;
    int capacity;
    int edge_index; // Index in graph[u]
};


int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N, M;
    if (!(cin >> N >> M)) return 0;

    Dinic dinic(N);
    vector<InputEdge> edges;

    // 1. Read Edges
    for (int i = 0; i < M; i++) {
        int u, v, c;
        cin >> u >> v >> c;
        // Store the index returned by addEdge to retrieve flow later
        int idx = dinic.addEdge(u, v, c);
        edges.push_back({u, v, c, idx});
    }

    int s, t;
    cin >> s >> t;

    // 2. Compute Max Flow
    cout << dinic.max_flow(s, t) << "\n";

    // 3. Print Flow on Each Edge
    for (const auto &e : edges) {
        long long flow = dinic.graph[e.u][e.edge_index].flow;
        cout << e.u << " " << e.v << " " << flow << "/" << e.capacity << "\n";
    }

    return 0;
}