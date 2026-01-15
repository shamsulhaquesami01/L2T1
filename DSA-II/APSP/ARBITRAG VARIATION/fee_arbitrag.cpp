#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>

using namespace std;

void solve() {
    int n;
    while (cin >> n) {
        map<string, int> mapStrToInt;
        for (int i = 0; i < n; i++) {
            string s; cin >> s;
            mapStrToInt[s] = i;
        }

        int m; cin >> m;
        
        // dist[i][j] stores the BEST exchange rate from i to j
        vector<vector<double>> dist(n, vector<double>(n, 0.0));

        // Initialize diagonals to 1.0 (1 unit of self is worth 1 unit)
        for(int i=0; i<n; i++) dist[i][i] = 1.0;

        for (int i = 0; i < m; i++) {
            string u_str, v_str;
            double rate, commission_pct;
            
            // Input: USD 1.5 2.0 AUD
            // Means: USD -> AUD rate is 1.5, but 2.0% fee is deducted
            cin >> u_str >> rate >> commission_pct >> v_str;
            
            int u = mapStrToInt[u_str];
            int v = mapStrToInt[v_str];

            // --- THE LOGIC VARIATION ---
            // Calculate Effective Rate immediately
            double fee_multiplier = (100.0 - commission_pct) / 100.0;
            double effective_rate = rate * fee_multiplier;

            dist[u][v] = max(dist[u][v], effective_rate);
        }

        // Standard Floyd-Warshall for Arbitrage (Max Product)
        for (int k = 0; k < n; k++) {
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    if (dist[i][k] * dist[k][j] > dist[i][j]) {
                        dist[i][j] = dist[i][k] * dist[k][j];
                    }
                }
            }
        }

        bool possible = false;
        for (int i = 0; i < n; i++) {
            if (dist[i][i] > 1.0) {
                possible = true;
                break; // Found a profitable loop
            }
        }

        if (possible) cout << "Yes (Profit possible despite fees)" << endl;
        else cout << "No" << endl;
    }
}

int main() {
    solve();
    return 0;
}