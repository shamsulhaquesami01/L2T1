// k ta connected components wala forest banaite bolse
// vertice - edge = component shongkha
// tree te n ta edge ar ekta component tai
// n - edge = 1 --> edge = n-1 hoy;

#include <iostream>
#include <vector>
#include <ctype.h>
#include <queue>
#include <climits>
#include<algorithm>
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

void KRUSKAL(vector<Edge> &edges, int n,int k)
{
    vector<pair<int, int>> result;
    int total=0;
    int edge_count =0;
    DSU dsu(n);
    sort(edges.begin(), edges.end());
    for (auto [u, v, w] : edges)
    {
        if (dsu.find(u) != dsu.find(v))
        {
            result.push_back({u, v});
            total+=w;
            dsu.unite(u, v);
            edge_count++;
            if(edge_count ==n-k) break;
        }
    }
    if(edge_count < n-k) cout<<"insufficient edges"<<endl;
    cout<<"Total weight "<<total<<endl;
    for (auto [u, v] : result)
    {
        cout << u << " " << v << endl;
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
        edges.push_back({u, v, w});
    }
    int k;
    cin >> k; // k ta component
    KRUSKAL(edges, n,k);
}

