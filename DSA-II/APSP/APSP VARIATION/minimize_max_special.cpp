/*Problem Statement: Two friends live at city A and city B. They want to meet at a city M such that the time the last person arrives is minimized. (i.e., Minimize max(dist[A][M],dist[B][M])).

The Logic: This doesn't change the FW algorithm itself, but changes how you query the result.

Run standard FW to get dist[][].

For a query (A,B), iterate through ALL possible meeting nodes k (from 1 to N).

Compute time = max(dist[A][k], dist[B][k]).

Track the minimum time.*/

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <climits>
using namespace std;
using ll = long long;
const ll INF = 1e18;

void solve(vector<vector<ll>> &dist, int n)
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
    cin >> n >> m;
    vector<vector<ll>> dist(n + 1, vector<ll>(n + 1, INF));
    for (int i = 1; i <= n; i++)
        dist[i][i] = 0;
    for (int i = 0; i < m; i++)
    {
        int a, b;
        ll w;
        cin >> a >> b >> w;
        dist[a][b] = min(dist[a][b], w);
    }
    solve(dist, n);
    int q;
    cin >> q;
    while (q--)
    {
        int A, B;
        cin >> A >> B;
        long long bestTime = INF;
        int bestMeetingNode = -1;

        for (int k = 1; k <= n; k++)
        {
            if (dist[A][k] == INF || dist[B][k] == INF)
                continue;

            // They arrive when the SLOWER person arrives
            long long arrivalTime = max(dist[A][k], dist[B][k]);

            if (arrivalTime < bestTime)
            {
                bestTime = arrivalTime;
                bestMeetingNode = k;
            }
        }

        if (bestMeetingNode != -1)
            cout << "Meet at " << bestMeetingNode << " Time: " << bestTime << endl;
        else
            cout << "Impossible" << endl;
    }
}

// g++ -std=c++17 dsa-2/APSP/OFFLINE/problem01.cpp -o solution && ./solution < dsa-2/APSP/OFFLINE/input.txt
