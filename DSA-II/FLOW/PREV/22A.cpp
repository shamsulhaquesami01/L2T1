#include <iostream>
#include <vector>
#include <queue>
#include <tuple>
#include <climits>
#include <algorithm>

using namespace std;

// --- YOUR TEMPLATE START ---
struct Edge {
    int to;
    int capacity; 
    int flow;
    int rev;
};

class MaxFlow {
public: // Made public to access graph for path printing
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

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, m;
    if (cin >> n >> m) {
        MaxFlow mf(n + 1);
        
        // 1. Build Graph with Capacity 1
        for (int i = 0; i < m; i++) {
            int u, v;
            cin >> u >> v;
            mf.addEdge(u, v, 1);
        }

        // 2. Calculate Max Disjoint Paths
        int max_paths = mf.edmondsKarp(1, n);
        cout << max_paths << endl;

        // 3. Path Reconstruction
        // We need to extract 'max_paths' number of paths
        vector<int> path;
        for (int k = 0; k < max_paths; k++) {
            int curr = 1;
            path.clear();
            path.push_back(1);

            while (curr != n) {
                // Find an edge with flow == 1
                for (auto &e : mf.graph[curr]) {
                    if (e.flow == 1 && e.capacity == 1) { // Check capacity to ensure it's a forward edge
                        e.flow = 0; // "Remove" flow so we don't traverse this again
                        curr = e.to;
                        path.push_back(curr);
                        break;
                    }
                }
            }

            // Print the found path
            cout << path.size() << endl;
            for (int i = 0; i < path.size(); i++) {
                cout << path[i] << (i == path.size() - 1 ? "" : " ");
            }
            cout << endl;
        }
    }
    return 0;
}