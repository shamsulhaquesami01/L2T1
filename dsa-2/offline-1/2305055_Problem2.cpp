#include <iostream>
#include <vector>
#include <climits>

using namespace std;
using ll = long long;

class Edge
{
public:
    int u, v;
    ll w;
    Edge(int u, int v, ll w)
    {
        this->u = u;
        this->v = v;
        this->w = w;
    }
};

void bellmanFord(int startNode, int N, int E, vector<Edge> &edges)
{
    // Initialization, (multi-source)
    vector<ll> dist(N+1, 0);
    vector<int> parent(N + 1, -1);

    // Relaxation ( N-1 times)
    for (int i = 1; i <= N - 1; i++)
    {
        for (const auto &edge : edges)
        {
            int u = edge.u;
            int v = edge.v;
            ll w = edge.w;
            if (dist[u] + w < dist[v])
            {
                dist[v] = dist[u] + w;
                parent[v] = u;
            }
        }
    }

    // Negative Cycle Check
    int cycle_node = -1;
    for (const auto &edge : edges)
    {
        int u = edge.u;
        int v = edge.v;
        ll w = edge.w;
        if (dist[u] + w < dist[v])
        {
            cycle_node = v;
            parent[v] = u;
            break; 
        }
    }

    if (cycle_node != -1)
    {
        vector<int> cycle;

        // backward N times
        for (int i = 0; i < N; ++i)
        {
            cycle_node = parent[cycle_node];
        }

        int curr = cycle_node;
        while (true)
        {
            cycle.push_back(curr);
            if (curr == cycle_node && cycle.size() > 1)
                break;
            curr = parent[curr];
        }

        // reverse
        reverse(cycle.begin(), cycle.end());
        for (size_t i = 0; i < cycle.size()-1; ++i)
        {
            cout << cycle[i] << " ";
        }
        cout << endl;
    }
    else
    {
        cout << "-1" << endl;
    }
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
        edges.push_back(Edge(u, v, w));
    }
    bellmanFord(1, n, m, edges);
}

//g++ -std=c++17 solution.cpp -o solution
//solution < input.txt

//g++ -std=c++17 solution.cpp -o solution && solution < input.txt
