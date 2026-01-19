// Helper BFS to mark component

#include<iostream>
#include<vector>
#include<ctype.h>
#include<queue>
#include<climits>
using namespace std;
class Edge
{
public:
    int from, to;
    int w;
    bool operator<(const Edge &other) const
    {
        return w < other.w;
    }
};
void markComponent(int start, int n, vector<vector<int>>& mst_adj, vector<bool>& visited, int broken_u, int broken_v) {
    queue<int> q;
    q.push(start);
    visited[start] = true;
    while(!q.empty()){
        int curr = q.front(); q.pop();
        for(int neighbor : mst_adj[curr]){
            // Don't cross the broken edge
            if((curr == broken_u && neighbor == broken_v) || (curr == broken_v && neighbor == broken_u)) continue;
            
            if(!visited[neighbor]){
                visited[neighbor] = true;
                q.push(neighbor);
            }
        }
    }
}

void solveEdgeRemoval(int n, vector<Edge>& all_edges, vector<Edge>& mst_edges, int broken_u, int broken_v) {
    // 1. Build Adjacency List for current MST
    vector<vector<int>> mst_adj(n + 1);
    long long current_mst_weight = 0;
    bool edge_in_mst = false;

    for(auto& e : mst_edges) {
        mst_adj[e.from].push_back(e.to);
        mst_adj[e.to].push_back(e.from);
        current_mst_weight += e.w;
        if ((e.from == broken_u && e.to == broken_v) || (e.from == broken_v && e.to == broken_u)) {
            edge_in_mst = true;
            current_mst_weight -= e.w; // Remove broken edge cost
        }
    }

    // If broken edge wasn't in MST, cost doesn't change
    if (!edge_in_mst) {
        cout << "Edge was not in MST. Cost remains: " << current_mst_weight << endl;
        return;
    }

    // 2. Identify Component U (Reach everything possible from u without crossing to v)
    vector<bool> in_component_u(n + 1, false);
    markComponent(broken_u, n, mst_adj, in_component_u, broken_u, broken_v);

    // 3. Find cheapest replacement edge crossing the cut
    long long min_replacement = 1e18;
    bool possible = false;

    for (auto& e : all_edges) {
        // Check if edge connects Component U to Component V (Not U)
        bool u_in = in_component_u[e.from];
        bool v_in = in_component_u[e.to];

        // XOR check: One end must be in U, the other NOT in U
        if (u_in != v_in) {
            // Can't use the broken edge itself
            if ((e.from == broken_u && e.to == broken_v) || (e.from == broken_v && e.to == broken_u)) continue;
            
            if (e.w < min_replacement) {
                min_replacement = e.w;
                possible = true;
            }
        }
    }

    if (possible)
        cout << "New MST Cost: " << current_mst_weight + min_replacement << endl;
    else
        cout << "Graph becomes disconnected!" << endl;
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m;
    cin >> n >> m;
    vector<Edge> edges;
    for (int i = 0; i < m; i++)
    {
        int u, v, w;
        cin >> u >> v >> w;
        edges.push_back({u, v, w});
    }
    vector<Edge> mst_edges; // assume given.
    int x,y; cin>>x>>y; //broken edge
    solveEdgeRemoval( n,edges, mst_edges, x, y);
}

