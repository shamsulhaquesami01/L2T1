#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <climits>
using namespace std;
using ll = long long;
const ll INF = 1e18;
ll solve(int remove, int n, vector<vector<ll> >& dist){
  
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
    ll sum =0;
    for (int i = 1; i <= n; i++)
        {
            for (int j = 1; j <= n; j++)
            {
                if(dist[i][j] != INF)
                sum += dist[i][j];
            }
        }
        if(remove != -1){
        for (int j = 1; j <=n; j++){
            dist[j][remove]=INF;
            dist[remove][j]=INF;
        }
    }
    return sum;

}

int main(){
    int n; cin>>n;
    vector<vector<ll> > dist(n + 1, vector<ll>(n + 1, INF));
    for (int i = 1; i <=n; i++)
    {
        for (int j = 1; j <=n; j++){
        ll w;
        cin >> w;
        dist[i][j] = min(dist[i][j], w);
        dist[j][i] = min(dist[j][i], w);
        }
    }
    solve(-1,n,dist);

}
//g++ -std=c++17 dsa-2/APSP/cp_algo_problems/Greg_and_Graph.cpp -o solution && ./solution < dsa-2/APSP/OFFLINE/input.txt 