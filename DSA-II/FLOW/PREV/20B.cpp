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
// --- TEMPLATE END ---

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int a,b; cin>>a>>b;
    int n_boys, m_girls, k_edges;
    if (cin >> n_boys >> m_girls >> k_edges) {
        
        // 1. Define Node Mapping
        // Source = 0
        // Boys = 1 to n_boys
        // Girls = n_boys + 1 to n_boys + m_girls
        // Sink = n_boys + m_girls + 1
        
        int S = 0;
        int T = n_boys + m_girls + 1;
        MaxFlow mf(T + 1);

        // 2. Connect Source -> Boys
        for(int i=1; i<=n_boys; i++) {
            mf.addEdge(S, i, b);
        }

        // 3. Connect Girls -> Sink
        for(int i=1; i<=m_girls; i++) {
            mf.addEdge(n_boys + i, T, b);
        }

        // 4. Connect Boys -> Girls (Input Edges)
        // Store these to print later
        // Just store 'u' (boy) and index of edge in 'u's list
        vector<pair<int, int>> potential_matches; 

        for (int i = 0; i < k_edges; i++) {
            int u, v;
            cin >> u >> v; // u = boy, v = girl
            
            // Map girl's ID to our new range
            int girl_node = n_boys + v;
            
            int idx = mf.addEdge(u, girl_node, a);
            potential_matches.push_back({u, idx});
        }

        // 5. Compute Max Matching
        cout << mf.edmondsKarp(S, T) << endl;

        // 6. Print Matches
        // Iterate through the input edges (Boys -> Girls)
        for (auto p : potential_matches) {
            int boy = p.first;
            int edge_idx = p.second;
            
            Edge e = mf.graph[boy][edge_idx];
            
            if (e.flow >0 && e.capacity>0) {
                // To get original Girl ID: e.to - n_boys
                cout <<"(" <<boy << " " << (e.to - n_boys) <<")->"<<e.flow<< endl;
            }
        }
    }
    return 0;
}