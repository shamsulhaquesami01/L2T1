#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <climits>
using namespace std;
using ll = long long;
const ll INF = 1e18;

void solve(vector<vector<ll> >& dist,int n)
{

    for (int k = 1; k <= n; k++)
    {
        for (int i = 1; i <= n; i++)
        {
            for (int j = 1; j <= n; j++)
            {
                if (dist[i][k] != INF && dist[k][j] != INF)
                {
                    dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
                }
            }
        }
    }
}
int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    ll n, m;
    cin >> n >> m ;
    vector<vector<ll> > dist(n + 1, vector<ll>(n + 1, INF));
    for (int i = 1; i <= n; i++)
        dist[i][i] = 0;
    for (int i = 0; i < m; i++)
    {
        int a, b;
        ll w;
        cin >> a >> b >> w;
        dist[a][b] = min(dist[a][b], w);
    }
    int y,z; cin>>y>>z;
    int mandatory_weight = dist[y][z];
    int q; cin>>q;
    for(int i=0; i<q; i++){
        int c,d; cin>>c>>d;
        if(mandatory_weight != INF && dist[c][y] != INF && dist[z][d] != INF)
            cout<<dist[c][y]+mandatory_weight+dist[z][d]<<endl;
        
        else cout<<"-1"<<endl;
    }
    

}

//g++ -std=c++17 dsa-2/APSP/OFFLINE/problem01.cpp -o solution && ./solution < dsa-2/APSP/OFFLINE/input.txt