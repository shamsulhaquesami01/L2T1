//normal mst, total cost koto hobe connect korte
//na korte paarle impossible


#include <iostream>
#include <vector>
#include <ctype.h>
#include <queue>
#include <algorithm>
using namespace std;
using ll =long long;
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
        parent.resize(n+1);
        size.resize(n+1, 1);
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
    int parent=0;
    vector<pair<int, int>> result;
    ll total=0;
    DSU dsu(n);
    sort(edges.begin(), edges.end());
    for (auto [u, v, w] : edges)
    {
        if (dsu.find(u) != dsu.find(v))
        {
            result.push_back({u, v});
            total+=w;
            dsu.unite(u, v);
            parent=dsu.find(u);
        }
    }
    for(int i=1; i<=n; i++){
        if(dsu.find(i)!=parent) {
            cout<<"IMPOSSIBLE"<<endl;
            return;
        }
    }
    cout<<total<<endl;
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
    KRUSKAL(edges, n);
}

