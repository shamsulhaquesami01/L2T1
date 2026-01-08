#include <iostream>
#include <vector>
#include <ctype.h>
#include <queue>
using namespace std;

using ll = long long;
const ll INF = 1e18;

class Edge
{
    public:
    int u, v;
    ll w;
    Edge(int u, int v, ll w){
        this->u=u; this->v=v; this->w=w;
    }
};

vector<ll> dijkstra(int start, vector<vector<pair<int, ll> > > &adj, int n)
{
    vector<ll> dist(n + 1, INF);
    dist[start] = 0;
    priority_queue<pair<ll, int>, vector<pair<ll, int> >, greater<pair<ll, int> > > pq;
    pq.push(make_pair(0, start));
    while (!pq.empty())
    {
        auto [d, u] = pq.top();
        pq.pop();
        if (d > dist[u])
            continue;
        for (auto [v, w] : adj[u])
        {
            if (dist[u] + w < dist[v])
            {
                dist[v] = dist[u] + w;
                pq.push(make_pair(dist[v], v));
            }
        }
    }
    return dist;
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;
    vector<Edge> edges;
    vector<vector<pair<int, ll> > > adj(n + 1), rev(n + 1);

    for (int i = 0; i < m; i++)
    {
        int a, b;
        ll c;
        cin >> a >> b >> c;
        edges.push_back(Edge(a, b, c));
        adj[a].push_back(make_pair(b, c));
        rev[b].push_back(make_pair(a, c));
    }
    vector<ll> dist1 = dijkstra(1, adj,n); 
    vector<ll> dist2 = dijkstra(n, rev,n); 

    ll ans = INF;
    for (Edge e : edges)
    {
        if (dist1[e.u] < INF && dist2[e.v] < INF)
        {
            ans = min(ans, dist1[e.u] + e.w / 2 + dist2[e.v]);
        }
    }

    cout << ans << "\n";
}

//g++ -std=c++17 solution.cpp -o solution
//solution < input.txt

//g++ -std=c++17 solution.cpp -o solution && solution < input.txt