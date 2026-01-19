#include<iostream>
#include<vector>
#include<ctype.h>
#include<queue>
#include<climits>
using namespace std;
class Edge
{
public:
    int from, to;
    int w;
    bool operator<(const Edge &other) const
    {
        return w > other.w;
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
void PRIMS(int n, vector<vector<pair<int, int> > >& adj, int source,vector<int>& parent){

    vector<int> dist(n, INT_MAX);
    vector< pair<int,int> > result;
    vector<bool>inMST(n,false);
    int total =0;
    dist[source]=0;
    priority_queue<pair<int,int>,vector<pair<int,int> >,greater<pair<int,int> > >pq;
    pq.push({0, source});
    while(!pq.empty()){
        auto [d,u] = pq.top(); pq.pop();
        if(d>dist[u]) continue;
        inMST[u]=true;
        if(parent[u]!=-1)result.push_back(make_pair(parent[u],u));
        total+=d;
        for(auto[v,w]: adj[u] ){
            if(w<dist[v] && inMST[v] == false){
                dist[v]=w;
                pq.push(make_pair(dist[v],v));
                parent[v]=u;
            }
        }
    }
    cout<<"Total weight "<<total<<endl;
    cout << "Root node " << source << endl; 
    for(auto[u,v] : result){
        cout<<u<<" "<<v<<endl;
    }
}
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
//g++ -std=c++17 maximum_spanning_tree.cpp -o solution && ./solution < input.txt