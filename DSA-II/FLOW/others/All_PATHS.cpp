#include <iostream>
#include <vector>
#include <queue>
#include <tuple>
#include <climits>
#include <algorithm>

using namespace std;

struct Edge
{
    int to;
    int capacity;
    int flow;
    int rev; // Index of the reverse edge
};

class MaxFlow
{
    int N;
    // graph is kept private, but we access it via internal methods
    vector<vector<Edge>> graph;

public:
    MaxFlow(int n) : N(n), graph(n) {}
    
    int addEdge(int u, int v, int cap)
    {
        // Forward edge: u -> v
        Edge a = {v, cap, 0, (int)graph[v].size()};
        // Backward edge: v -> u
        Edge b = {u, 0, 0, (int)graph[u].size()};
        graph[u].push_back(a);
        graph[v].push_back(b);
        return (int)graph[u].size() - 1;
    }

    int bfs(int s, int t, vector<pair<int, int>> &parent)
    {
        fill(parent.begin(), parent.end(), make_pair(-1, -1));
        queue<int> q;
        q.push(s);
        parent[s] = {s, -1};

        while (!q.empty())
        {
            int u = q.front();
            q.pop();
            for (int i = 0; i < graph[u].size(); i++)
            {
                Edge &e = graph[u][i];
                if (parent[e.to].first == -1 && e.capacity > e.flow)
                {
                    parent[e.to] = {u, i};
                    if (e.to == t)
                        return 1;
                    q.push(e.to);
                }
            }
        }
        return 0;
    }

    int edmondsKarp(int s, int t)
    {
        int maxFlow = 0;
        vector<pair<int, int>> parent(N);

        while (bfs(s, t, parent))
        {
            int pathFlow = INT_MAX;
            for (int v = t; v != s;)
            {
                auto [u, idx] = parent[v];
                pathFlow = min(pathFlow, graph[u][idx].capacity - graph[u][idx].flow);
                v = u;
            }
            for (int v = t; v != s;)
            {
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

    Edge getEdge(int u, int idx)
    {
        return graph[u][idx];
    }

    // --- NEW METHOD: Decompose Flow into Paths ---
    void decomposeAndPrintPaths(int s, int t) {
        cout << "\n--- Flow Decomposition (Paths) ---\n";
        
        while (true) {
            // Use BFS to find a path using ONLY edges with current flow > 0
            vector<int> parent_node(N, -1);
            vector<int> parent_edge_idx(N, -1);
            queue<int> q;
            
            q.push(s);
            parent_node[s] = s; // Mark source as visited

            bool pathFound = false;
            while(!q.empty()) {
                int u = q.front(); q.pop();
                if(u == t) {
                    pathFound = true;
                    break;
                }
                for(int i = 0; i < graph[u].size(); i++) {
                    Edge &e = graph[u][i];
                    // Crucial Check: e.flow > 0 (We are following the water now)
                    if(parent_node[e.to] == -1 && e.flow > 0) {
                        parent_node[e.to] = u;
                        parent_edge_idx[e.to] = i;
                        q.push(e.to);
                    }
                }
            }

            if(!pathFound) break; // No more flow to decompose

            // 1. Find bottleneck flow on this specific path
            int bottleneck = INT_MAX;
            int curr = t;
            vector<int> path;
            
            while(curr != s) {
                path.push_back(curr);
                int prev = parent_node[curr];
                int idx = parent_edge_idx[curr];
                bottleneck = min(bottleneck, graph[prev][idx].flow);
                curr = prev;
            }
            path.push_back(s);
            reverse(path.begin(), path.end());

            // 2. Print the path info
            cout << "Flow: " << bottleneck << " | Path: ";
            for(size_t i = 0; i < path.size(); i++) {
                cout << path[i] << (i == path.size() - 1 ? "" : " -> ");
            }
            cout << endl;

            // 3. Subtract this bottleneck from the graph so we find next paths
            // (Note: This destructively modifies the flow in the graph, 
            // so we do this AFTER printing the edge list in main)
            curr = t;
            while(curr != s) {
                int prev = parent_node[curr];
                int idx = parent_edge_idx[curr];
                graph[prev][idx].flow -= bottleneck;
                curr = prev;
            }
        }
    }
};

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N, M;
    if (!(cin >> N >> M))
        return 0;

    MaxFlow mf(N+1);
    vector<tuple<int, int, int, int>> edges;

    for (int i = 0; i < M; i++)
    {
        int u, v, c;
        cin >> u >> v >> c;
        int idx = mf.addEdge(u, v, c);
        edges.emplace_back(u, v, c, idx);
    }

    int s, t;
    cin >> s >> t;

    // 1. Calculate Max Flow
    cout << mf.edmondsKarp(s, t) << "\n";

    // 2. Print Flows on Edges (Required by original problem)
    for (const auto &[u, v, c, idx] : edges)
    {
        Edge e = mf.getEdge(u, idx);
        cout << u << " " << v << " " << e.flow << "/" << c << "\n";
    }

    // 3. Print All Paths (Flow Decomposition)
    // NOTE: This must be called LAST because it reduces the flow in the graph to 0.
    mf.decomposeAndPrintPaths(s, t);

    return 0;
}