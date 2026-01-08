#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
using ll = long long;

const ll INF = 1e18;

void solve() {
    int n;
    if (!(cin >> n)) return;

    // 1. Read the Adjacency Matrix
    // Note: We'll update this matrix in-place.
    vector<vector<ll>> dist(n + 1, vector<ll>(n + 1));
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            cin >> dist[i][j];
        }
    }

    // 2. Read the deletion order
    vector<int> del(n);
    for (int i = 0; i < n; i++) {
        cin >> del[i];
    }

    // 3. Process in REVERSE
    vector<ll> ans;
    vector<bool> active(n + 1, false);

    // Iterate backwards through the deletion list
    for (int k_idx = n - 1; k_idx >= 0; k_idx--) {
        int k = del[k_idx]; // The node we are "activating"
        active[k] = true;

        // Run ONE iteration of Floyd-Warshall using 'k' as the hub
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= n; j++) {
                dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
            }
        }

        // Sum distances ONLY between active nodes
        ll current_sum = 0;
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= n; j++) {
                if (active[i] && active[j]) {
                    current_sum += dist[i][j];
                }
            }
        }
        ans.push_back(current_sum);
    }

    // 4. Reverse the answers to match the input order
    reverse(ans.begin(), ans.end());

    for (int i = 0; i < n; i++) {
        cout << ans[i] << (i == n - 1 ? "" : " ");
    }
    cout << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    solve();
    return 0;
}