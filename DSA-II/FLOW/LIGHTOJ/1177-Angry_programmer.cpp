// You, a programmer of an important software house, have been fired because you didn't solve an important problem that was assigned to you. 
// You are very furious and want to take revenge on your boss, breaking the communication between his computer and the central server.
// The computer of your boss and the central server are in the same network, 
// which is composed of many machines (computers) and wires linking pairs of those machines. 
// There is at most one wire between any pair of machines and there can be pairs of machines without a wire between them.
// To accomplish your objective, you can destroy machines and wires, 
// but you can't destroy neither the computer of your boss, nor the central server, 
// because those machines are monitored by security cameras. You have estimated the cost of blowing up each machine and 
// the cost of cutting each wire in the network.
// You want to determine the minimum cost of interrupting the communication between your boss'
//  computer and the central server. Two computers A and B can communicate if there is a sequence 
//  of undestroyed machines x1, x2, ..., xn such that x1 = A, xn = B and xi is linked with xi+1 with an uncut wire (for each 1 ≤ i < n)

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

void solve(int t) {
    int M, W; 
    cin >> M >> W;
    
    int S = 1, T = M + M; 
    MaxFlow mf(2 * M + 1);

    // Node costs
    mf.addEdge(1, 1+M, INT_MAX); 
    mf.addEdge(M, M+M, INT_MAX); 
    

    for(int i=2; i<M; i++) {
        int cost; cin >> cost;
        mf.addEdge(i, i+M, cost);
    }
    
    // Edges
    for(int i=0; i<W; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        mf.addEdge(u+M, v, w);
        mf.addEdge(v+M, u, w);
    }

    cout << "Case " << t << ": " << mf.edmondsKarp(S, T) << endl;
}
int main() {
    int t;
    cin >> t;
    for (int i = 1; i <= t; i++) {
        solve(i);
    }
}