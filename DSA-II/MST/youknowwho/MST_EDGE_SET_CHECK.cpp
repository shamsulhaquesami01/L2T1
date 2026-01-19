#include <iostream>
#include <vector>
#include <ctype.h>
#include <queue>
#include <climits>
#include <algorithm>
using namespace std;
using ll = long long;
class Edge
{
public:
    int from, to;
    ll w;
    bool operator<(const Edge &other) const
    {
        return w < other.w;
    }
};
class DSU
{

    vector<int> parent;
    vector<int> size;

public:
    DSU(int n)
    {
        parent.resize(n + 1);
        size.resize(n + 1, 1);
        for (int i = 1; i <= n; i++)
        {
            parent[i] = i;
        }
    }

    int find(int x)
    {
        if (parent[x] != x)
        {
            parent[x] = find(parent[x]);
        }
        return parent[x];
    }

    void unite(int x, int y)
    {
        int rootX = find(x);
        int rootY = find(y);

        if (rootX == rootY)
            return;

        if (size[rootX] < size[rootY])
        {
            parent[rootX] = rootY;
            size[rootY] += size[rootX];
        }
        else
        {
            parent[rootY] = rootX;
            size[rootX] += size[rootY];
        }
    }

    bool connected(int x, int y)
    {
        return find(x) == find(y);
    }

    int getSize(int x)
    {
        return size[find(x)];
    }
};

ll KRUSKAL(vector<Edge> &edges, int n)
{
    ll total = 0;
    DSU dsu(n);
    sort(edges.begin(), edges.end());
    for (auto [u, v, w] : edges)
    {
        if (dsu.find(u) != dsu.find(v))
        {
            total += w;
            dsu.unite(u, v);
        }
    }
    if(dsu.find(1)!=dsu.find(n)) return LLONG_MIN;
    return total;
}
ll kruskal_forced(vector<Edge> &edges, int n, vector<Edge> input_order, vector<int> list)
{
    ll total = 0;
    DSU dsu(n);
    sort(edges.begin(), edges.end());
    for (int i : list)
    {
        if (dsu.find(input_order[i].from) != dsu.find(input_order[i].to))
        {
            total += input_order[i].w;
            dsu.unite(input_order[i].from, input_order[i].to);
        }
    }
    for (auto [u, v, w] : edges)
    {
        if (dsu.find(u) != dsu.find(v))
        {
            total += w;
            dsu.unite(u, v);
        }
    }
    return total;
}
int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m, q;
    cin >> n >> m >> q;
    vector<Edge> edges;
    for (int i = 0; i < m; i++)
    {
        int u, v;
        ll w;
        cin >> u >> v >> w;
        edges.push_back({u, v, w});
    }
    vector<Edge> input_order = edges;
    ll pre = KRUSKAL(edges, n);
    while (q--)
    {
        int length;
        cin >> length;
        vector<int> edges_list;
        while (length--)
        {
            int x;
            cin >> x;
            edges_list.push_back(x - 1);
        }
        ll final = kruskal_forced(edges, n, input_order, edges_list);
        if (final == pre && pre != LLONG_MIN)
            cout << "YES" << endl;
        else
            cout << "NO" << endl;
    }
}
