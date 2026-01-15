#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <cmath>   // Required for log()
#include <climits>

using namespace std;

const double INF = 1e9; // A sufficiently large number

void solve()
{
    int n;
    // Check if input exists (essential for some online judges)
    if (!(cin >> n)) return;

    map<string, int> currency;
    for (int i = 0; i < n; i++)
    {
        string s;
        cin >> s;
        currency[s] = i;
    }

    // Initialize with INF (Minimization problem)
    vector<vector<double>> dist(n, vector<double>(n, INF));

    // Distance to self is 0 in the "log world" (log(1.0) = 0)
    for (int i = 0; i < n; i++)
        dist[i][i] = 0.0;

    int m;
    cin >> m;
    for (int i = 0; i < m; i++)
    {
        string from, to;
        double rate;
        cin >> from >> rate >> to;
        int a = currency[from];
        int b = currency[to];

        // LOGIC CHANGE 1: Convert rate to negative log weight
        // We use min() in case multiple rates exist for same pair
        dist[a][b] = min(dist[a][b], -log(rate));
    }

    // LOGIC CHANGE 2: Standard Floyd-Warshall for Shortest Path (Min Sum)
    for (int k = 0; k < n; k++)
    {
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                // Check connectivity to avoid adding to INF
                if (dist[i][k] != INF && dist[k][j] != INF) {
                    dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
                }
            }
        }
    }

    // LOGIC CHANGE 3: Check for Negative Cycles
    for (int i = 0; i < n; i++)
    {
        // If distance to self is negative, the product of rates > 1.0
        // Use a small epsilon for float comparison safety
        if (dist[i][i] < -1e-9)
        {
            cout << "Yes" << endl;
            return;
        }
    }
    cout << "No" << endl;
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    solve();
    return 0;
}
