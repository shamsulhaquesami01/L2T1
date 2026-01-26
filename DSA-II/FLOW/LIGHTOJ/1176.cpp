

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
int sizeToId(string s) {
    if(s == "XXL") return 1; if(s == "XL") return 2; if(s == "L") return 3;
    if(s == "M") return 4; if(s == "S") return 5; if(s == "XS") return 6;
    return 0;
}

void solve(int t) {
    int N, M; // N shirts (multiple of 6), M contestants
    cin >> N >> M;
    
    // Nodes: S(0), Contestants(1..M), Sizes(M+1..M+6), T(M+7)
    int S = 0, T = M + 7;
    MaxFlow mf(T + 1);
    
    int limit_per_size = N / 6;

    // Sizes to Sink
    for(int i=1; i<=6; i++) mf.addEdge(M+i, T, limit_per_size);

    for(int i=1; i<=M; i++) {
        string s1, s2;
        cin >> s1 >> s2;
        mf.addEdge(S, i, 1); // Contestant needs 1 shirt
        mf.addEdge(i, M + sizeToId(s1), 1);
        mf.addEdge(i, M + sizeToId(s2), 1);
    }

    if(mf.edmondsKarp(S, T) == M) cout << "Case " << t << ": YES" << endl;
    else cout << "Case " << t << ": NO" << endl;
}

int main() {
    int t;
    cin >> t;
    for (int i = 1; i <= t; i++) {
        solve(i);
    }
}