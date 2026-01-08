#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <climits>
using namespace std;
using ll = long long;
const ll INF = 1e18;
void solve()
{

    ll n, m; // n node; 1 theke n porjonto; n vector 
    cin >> n >> m ;
    vector<vector<ll> > dist(n + 1, vector<ll>(n + 1, INF));
    for (int i = 1; i <= n; i++)
        dist[i][i] = 0;
    for (int i = 0; i < m; i++)
    {
        int a, b;
        cin >> a >> b;
        dist[a][b] = 1;
    }
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
    double sum =0;
    int count =0;
    for(int i=1; i<=n; i++){
        for(int j=1; j<=n; j++){
            if(dist[i][j] != INF){
              sum += dist[i][j];
              
            }
             if(dist[i][j] != INF && dist[i][j] !=0){
                  count+=1;
             }
        }
    }
    cout<<sum<<endl;
    cout<<count<<endl;
    cout<<sum/count<<endl;
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    solve();
}

//g++ -std=c++17 dsa-2/APSP/cp_algo_problems/Page_Hopping.cpp -o solution && ./solution < dsa-2/APSP/OFFLINE/input.txt