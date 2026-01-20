#include <iostream>
#include <vector>
#include <ctype.h>
#include <queue>
#include <climits>
#include <algorithm>
using namespace std;
class X
{
public:
    int x, position;
    bool operator<(const X &other) const
    {
        return x < other.x;
    }
};
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
class DSU
{

    vector<int> parent;
    vector<int> size;

public:
    DSU(int n)
    {
        parent.resize(n);
        size.resize(n, 1);
        for (int i = 0; i < n; i++)
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

void KRUSKAL(vector<Edge> &edges, int n)
{
    vector<pair<int, int>> result;
    int total = 0;
    DSU dsu(n);
    sort(edges.begin(), edges.end());
    for (auto [u, v, w] : edges)
    {
        if (dsu.find(u) != dsu.find(v))
        {
            result.push_back({u, v});
            total += w;
            dsu.unite(u, v);
        }
    }
    cout << "Total weight " << total << endl;
    for (auto [u, v] : result)
    {
        cout << u << " " << v << endl;
    }
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    cin >> n;
    vector<Edge> edges;
    vector<X> x_co, y_co, z_co;
    for (int i = 0; i < n; i++)
    {
        int a, b, c;
        cin >> a >> b >> c;
        x_co.push_back({a, i});
        y_co.push_back({b, i});
        z_co.push_back({c, i});
    }
    sort(x_co.begin(), x_co.end());
    sort(y_co.begin(), y_co.end());
    sort(z_co.begin(), z_co.end());
    for (int i = 0; i < x_co.size() - 1; i++)
    {
        int j = i + 1;
        edges.push_back(
            {x_co[i].position, x_co[j].position, abs(x_co[i].x - x_co[j].x)});
    }
    for (int i = 0; i < y_co.size() - 1; i++)
    {
        int j = i + 1;
        edges.push_back(
            {y_co[i].position, y_co[j].position, abs(y_co[i].x - y_co[j].x)});
    }
    for (int i = 0; i < z_co.size() - 1; i++)
    {
        int j = i + 1;
        edges.push_back(
            {z_co[i].position, z_co[j].position, abs(z_co[i].x - z_co[j].x)});
    }
    KRUSKAL(edges, n);
}
