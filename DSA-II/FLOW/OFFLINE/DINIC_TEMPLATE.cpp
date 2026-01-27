#include <vector>
#include <queue>
#include <algorithm>
#include <climits>

using namespace std;

struct Edge {
    int to;
    long long capacity;
    long long flow;
    int rev; // Index of reverse edge
};

class Dinic {
    int N;
    vector<vector<Edge>> graph;
    vector<int> level; // Level graph (distance from source)
    vector<int> ptr;   // Next edge index to explore (optimization)

public:
    Dinic(int n) : N(n), graph(n), level(n), ptr(n) {}

    void addEdge(int u, int v, long long cap) {
        Edge a = {v, cap, 0, (int)graph[v].size()};
        Edge b = {u, 0, 0, (int)graph[u].size()};
        graph[u].push_back(a);
        graph[v].push_back(b);
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