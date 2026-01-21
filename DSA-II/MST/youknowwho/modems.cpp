/*
A Oruro province wants ALL the towns to have Internet access and to communicate with each other by at least one channel (not necessarily direct). 
The engineer in charge asks you to help him determine the minimum cost of providing this access.
There are N towns in total (1 ≤ N ≤ 1000). UTP can be used only up to range R (1 ≤ R ≤ 10000). 
If this distance is exceeded, optical fiber must be used instead. 
The unit cost of using UTP cable is U and the unit price of the optical fiber is V (U ≤ V; 1 ≤ U, V ≤ 10).
 There are also W satellite modems purchased (1 ≤ W < N). These satellite modems can be placed in any town. 
 Satellite modem will allow a town to use the internet and communicate with any other modem in the provinc
*/

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
    int w; int type;
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
int calcdistnace(pair<int,int> A, pair<int,int> B){
    return abs(sqrt((A.first-B.first)*(A.first-B.first)+(A.second-B.second)*(A.second-B.second)));
}
void KRUSKAL(vector<Edge> &edges, int n,int k, int U)
{
    vector<pair<int, int>> result;
    int total_U=0;
    int total_V=0;
    int edge_count =0;
    DSU dsu(n);
    sort(edges.begin(), edges.end());
    for (auto [u, v, w,type] : edges)
    {
        if (dsu.find(u) != dsu.find(v))
        {
            result.push_back({u, v});
            if(type==1) total_U+=w;
            else total_V +=w;
            dsu.unite(u, v);
            edge_count++;
            if(edge_count ==n-k) break;
        }
    }
    cout<<"U "<<total_U<<"V "<<total_V<<endl;
    for (auto [u, v] : result)
    {
        cout << u << " " << v << endl;
    }
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int N,R,W,U,V;
    cin>>N>>R>>W>>U>>V;
    vector<Edge> edges;
    vector<pair<int,int> > towns(N);
    for (int i = 0; i < N; i++)
    {
        int x,y;cin>>x>>y;
        towns[i]={x,y};
    }
    for(int i =0; i<towns.size(); i++){
        for(int j =i+1; j<towns.size(); j++){
            int x = calcdistnace(towns[i],towns[j]);
            if(x<=R) edges.push_back({i,j,x*U,1});
            else edges.push_back({i,j,x*V,2});
        }
    }
    KRUSKAL(edges, N, W,U);
}

