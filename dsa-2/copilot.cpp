#include<iostream>
#include<vector>
#include<ctype.h>
#include<queue>
#include<climits>
using namespace std;
int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int R, C;
    if (!(cin >> R >> C))
        return 0;
    vector<vector<long long>> a(R + 1, vector<long long>(C + 1));
    for (int i = 1; i <= R; i++)
        for (int j = 1; j <= C; j++)
            cin >> a[i][j];
    vector<vector<long long>> dp(R + 1, vector<long long>(C + 1, -1e18));
    vector<vector<pair<int, int>>> parent(R + 1, vector<pair<int, int>>(C + 1, {-1, -1}));
    dp[1][1] = a[1][1];
    for (int i = 1; i <= R; i++)
    {
        for (int j = 1; j <= C; j++)
        {
            if (i > 1 && dp[i - 1][j] + a[i][j] > dp[i][j])
            {
                dp[i][j] = dp[i - 1][j] + a[i][j];
                parent[i][j] = {i - 1, j};
            }
            if (j > 1 && dp[i][j - 1] + a[i][j] > dp[i][j])
            {
                dp[i][j] = dp[i][j - 1] + a[i][j];
                parent[i][j] = {i, j - 1};
            }
        }
    }
    cout << dp[R][C] << "\n";
    vector<pair<int, int>> path;
    for (int i = R, j = C; i != -1;)
    {
        path.push_back({i, j});
        auto p = parent[i][j];
        i = p.first;
        j = p.second;
    }
    reverse(path.begin(), path.end());
    for (size_t k = 0; k < path.size(); k++)
    {
        if (k)
            cout << " ";
        cout << "(" << path[k].first << "," << path[k].second << ")";
    }
    cout << "\n";
    return 0;
}
