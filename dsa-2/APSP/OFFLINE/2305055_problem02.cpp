#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <algorithm>
#include <climits>
using namespace std;
using ll = long long;
void solve()
{
    int n;
    cin >> n;
    map<string, int> currency;
    for (int i = 0; i < n; i++)
    {
        string s;
        cin >> s;
        currency[s] = i;
    }
    vector<vector<double>> dist(n, vector<double>(n, 0.0));
    for (int i = 0; i < n; i++)
        dist[i][i] = 1.0;
    int m;
    cin >> m;
    for (int i = 0; i < m; i++)
    {
        string from, to;
        double rate;
        cin >> from >> rate >> to;
        int a = currency[from];
        int b = currency[to];
        dist[a][b] = rate;
    }

    for (int k = 0; k < n; k++)
    {
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {

                dist[i][j] = max(dist[i][j], dist[i][k] * dist[k][j]);
            }
        }
    }

    for (int i = 0; i < n; i++)
    {
        if (dist[i][i] > 1.0)
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
}

// g++ -std=c++17 dsa-2/APSP/OFFLINE/problem02.cpp -o solution && ./solution < dsa-2/APSP/OFFLINE/input.txt