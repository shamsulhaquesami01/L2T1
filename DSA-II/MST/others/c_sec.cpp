#include <iostream>
#include <vector>
#include <ctype.h>
#include <queue>
#include <climits>
#include<algorithm>
using namespace std;
class Point{
    public:
    int x,y,z;
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
    int total=0;
    DSU dsu(n);
    sort(edges.begin(), edges.end());
    for (auto [u, v, w] : edges)
    {
        if (dsu.find(u) != dsu.find(v))
        {
            result.push_back({u, v});
            total+=w;
            dsu.unite(u, v);
        }
    }
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
    int n;
    cin >> n ;
    vector<Edge> edges;
    vector<Point> points;
    for (int i = 0; i < n; i++)
    {
      int a,b,c;
      cin>>a>>b>>c;
      points.push_back({a,b,c});
    }
    for(int i=0; i<n; i++){
        for(int j=i+1; j<n; j++){
            edges.push_back({i,j,
            min(
            min(
                abs(points[i].x-points[j].x),
                abs(points[i].y-points[j].y)),
                abs(points[i].z-points[j].z)
            
            )});
        }
    }
    KRUSKAL(edges, n);
}

