#include<iostream>
#include<vector>
#include<ctype.h>
#include<queue>
#include<algorithm>
using namespace std;
using ll = long long;

class Edge
{
public:
    int from, to;
    ll w;
    // Sorting Descending (Largest weight first) 
    bool operator<(const Edge &other) const
    {
        return w > other.w;
    }
};

class DSU{
    vector<int> parent;
    vector<int> size;

public:
    DSU(int n)
    {
        // FIX 1: Resize to n + 1 because input is 1-based (1 to N)
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
};

void KRUSKAL(vector<Edge> &edges, int n)
{
    DSU dsu(n);
    sort(edges.begin(), edges.end()); 

    for (auto [u, v, w] : edges)
    {
        if (dsu.find(u) != dsu.find(v))
        {
            dsu.unite(u, v);
            // Since we process edges from largest to smallest, 
            // the first time 1 and N connect, 'w' is the bottleneck (answer).
            if(dsu.find(1) == dsu.find(n)) {
                cout << w << endl;
                return; 
            }
        }
    }
    cout << -1 << endl;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
        int n,m;
        cin>>n>>m;
        vector<Edge> edges;
        for(int i=0; i<m; i++){
            int u,v;
            ll w;
            cin>>u>>v>>w;
            edges.push_back({u, v, w});
        }
        KRUSKAL(edges,n);
    }


// g++ -std=c++17 maximizing_minimum.cpp -o solution && ./solution < input.txt