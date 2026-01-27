
// Petya has a simple graph (that is, a graph without loops or multiple edges)
//  consisting of 𝑛 vertices and 𝑚 edges.
// The weight of the 𝑖-th vertex is 𝑎𝑖.The weight of the 𝑖-th edge is 𝑤𝑖.
// A subgraph of a graph is some set of the graph vertices and some set of the graph edges. 
// The set of edges must meet the condition: both ends of each edge from the set must
// belong to the chosen set of vertices.
// The weight of a subgraph is the sum of the weights of its edges, 
// minus the sum of the weights of its vertices. You need to find the 
// maximum weight of subgraph of given graph. The given graph does not contain loops and multiple edges


#include <iostream>
#include <vector>
#include <queue>
#include <tuple>
#include <climits>
#include <algorithm>

using namespace std;

struct Edge {
    int to;
    long long capacity; 
    long long flow;
    int rev;
};

class MaxFlow {
public:
    int N;
    vector<vector<Edge>> graph;

    MaxFlow(int n) : N(n), graph(n) {}

    int addEdge(int u, int v, long long cap) {
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

    long long edmondsKarp(int s, int t) {
        long long maxFlow = 0;
        vector<pair<int,int>> parent(N);
        while (bfs(s, t, parent)) {
            long long pathFlow = LLONG_MAX; 
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

void solve() {
    int n, m;
    if (!(cin >> n >> m)) return;

    // Node Mapping:
    // Source = 0
    // Vertices (Graph Nodes) = 1 to n
    // Edges (Graph Edges) = n+1 to n+m
    // Sink = n + m + 1
    
    int S = 0;
    int T = n + m + 1;
    MaxFlow mf(T + 1);

    // 1. Read Vertex Weights (Costs)
    // Create link: Vertex -> Sink with capacity = Cost
    for (int i = 1; i <= n; i++) {
        long long cost;
        cin >> cost;
        mf.addEdge(i, T, cost);
    }

    long long total_edge_weight = 0;

    // 2. Read Edges (Profits)
    // Create link: Source -> EdgeNode with capacity = Profit
    // Create link: EdgeNode -> Vertex u with capacity = INF
    // Create link: EdgeNode -> Vertex v with capacity = INF
    for (int i = 1; i <= m; i++) {
        int u, v;
        long long w;
        cin >> u >> v >> w;
        
        total_edge_weight += w;
        
        int edge_node = n + i;
        
        // Potential Profit
        mf.addEdge(S, edge_node, w);
        
        // Dependencies (Infinite capacity so we never cut here)
        mf.addEdge(edge_node, u, LLONG_MAX);
        mf.addEdge(edge_node, v, LLONG_MAX);
    }

    // 3. Calculate Result
    // Max Profit = Total Possible Profit - Minimum Loss (Max Flow)
    long long min_loss = mf.edmondsKarp(S, T);
    cout << total_edge_weight - min_loss << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    solve();
    return 0;
}