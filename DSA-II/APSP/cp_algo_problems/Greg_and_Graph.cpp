#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <climits>
using namespace std;
using ll = long long;
const ll INF = 1e18;
ll solve(int n, vector<vector<ll>> &dist)
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
    ll sum = 0;
    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= n; j++)
        {
            if (dist[i][j] != INF && dist[i][j] != 0)
                sum += dist[i][j];
        }
        cout << endl;
    }
    return sum;
}

int main()
{
    int n;
    cin >> n;
    vector<vector<ll>> dist(n + 1, vector<ll>(n + 1, INF));
    vector<vector<ll>> edge(n + 1, vector<ll>(n + 1, INF));
    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= n; j++)
        {
            ll w;
            cin >> w;
            edge[i][j] = w;
        }
    }

    vector<int> remove;
    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= n; j++)
        {
            dist[i][j] = edge[i][j];
        }
    }
    cout << solve(n, dist) << endl;
    
    for (int k = 0; k < n - 1; k++)
    {
        int x;
        cin >> x;
        remove.push_back(x);
        for (int i = 1; i <= n; i++)
        {
            for (int j = 1; j <= n; j++)
            {
                dist[i][j] = edge[i][j];
            }
        }
        for (int x : remove)
        {
            for (int i = 1; i <= n; i++)
            {

                dist[i][x] = INF;
                dist[x][i] = INF;
            }
        }
        cout << solve(n, dist) << endl;
    }
}
// g++ -std=c++17 dsa-2/APSP/cp_algo_problems/Greg_and_Graph.cpp -o solution && ./solution < dsa-2/APSP/OFFLINE/input.txt