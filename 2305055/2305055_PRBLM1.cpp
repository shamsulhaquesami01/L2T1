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

    cout << mf.edmondsKarp(s, t) << "\n";

    for (const auto &[u, v, c, idx] : edges)
    {
        Edge e = mf.getEdge(u, idx);
        cout << u << " " << v << " " << e.flow << "/" << c << "\n";
    }

    return 0;
}