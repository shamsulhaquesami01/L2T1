#include <iostream>
#include <vector>
#include <climits> // For INT_MAX

using namespace std;

struct Edge
{
    int u, v;
    long long w;
};

void solve()
{
    int n;
    cin >> n;
    vector<int> business(n+1);
    for (int i = 1; i <= n; i++)
    {
        cin >> business[i];
    }
    int m;
    cin >> m;
    vector<Edge> edges;
    for (int i = 0; i < m; i++)
    {
        int u, v;
        cin >> u >> v;
        long long diff = business[v] - business[u];
        long long w = diff * diff * diff;

        edges.push_back({u, v, w});
    }

    vector<long long> dist(n + 1, LLONG_MAX);
    dist[1] = 0;
    for (int i = 0; i < n - 1; i++)
    {
        for (const auto &edge : edges)
        {
            int u = edge.u;
            int v = edge.v;
            long long w = edge.w;
            if (dist[u] != LLONG_MAX && dist[u] + w < dist[v])
            {
                dist[v] = dist[u] + w;
            }
        }
    }
    int q;
    cin >> q;
    for (int i = 0; i < q; i++)
    {
        int d;
        cin >> d;
        if (dist[d] == LLONG_MAX || dist[d] < 3)
            cout << "?" << endl;
        else
            cout << dist[d] << endl;
    }
}

int main()
{

    int t;
    cin >> t;
    for (int i = 0; i < t; i++)
    {
        cout << "Case " << i + 1 << ":" << endl;
        solve();
    }
}
