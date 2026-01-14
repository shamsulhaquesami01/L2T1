
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <climits>
using namespace std;
using ll = long long;
const ll INF = 1e18;

void solve(vector<vector<ll> >& dist,int n,int badNode)
{

  for (int k = 1; k <= n; k++) {
        
        // CONSTRAINT 2: Quarantined City
        // If 'k' is the bad node, we cannot use it as an intermediate point.
        if (k == badNode) continue; 

        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= n; j++) {
                // Also ensures we don't start/end at badNode implicitly
                // (though queries usually handle that check too)
                if (dist[i][k] != INF && dist[k][j] != INF) {
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
    int badnonde; cin>>badnonde;
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
    solve(dist,n,badnonde);
    int q; cin>>q;
    for (int i = 0; i < q; i++)
    {
        int a, b;
        cin >> a >> b;
        if (dist[a][b] != INF)
            cout << dist[a][b]<<endl;
        else
            cout << -1<<endl;
    }
    

}






