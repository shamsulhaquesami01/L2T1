/*
Concept: Similar to the "One Free Edge" problem.
 Problem Statement: You are a trader with a secret advantage. 
 You have a "Golden Voucher" that allows you to get a 10% bonus rate (multiply by 1.10)
  on exactly one exchange in your sequence. Determine if arbitrage is possible 
  specifically by using this voucher exactly once. (i.e., The arbitrage might not exist with normal rates, 
  but does exist if you boost one specific edge).

*/

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <iomanip>

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

        // dist[0] = No Voucher, dist[1] = 1 Voucher Used
        vector<vector<vector<double>>> dist(2, vector<vector<double>>(n, vector<double>(n, 0.0)));

        // Initialize diagonals (Identity profit is 1.0)
        for (int i = 0; i < n; i++) {
            dist[0][i][i] = 1.0;
            // dist[1][i][i] stays 0.0 because you can't use a voucher on "no movement"
        }

        double BONUS = 1.10; // 10% Bonus

        for (int i = 0; i < m; i++) {
            string u_str, v_str;
            double rate;
            cin >> u_str >> rate >> v_str;
            int u = mapStrToInt[u_str];
            int v = mapStrToInt[v_str];

            // 1. Normal path
            dist[0][u][v] = max(dist[0][u][v], rate);
            
            // 2. Path if we use the voucher on THIS specific edge
            dist[1][u][v] = max(dist[1][u][v], rate * BONUS);
        }

        // Floyd-Warshall with State
        for (int k = 0; k < n; k++) {
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    
                    // Case 1: Normal Path + Normal Path = Normal Path
                    if (dist[0][i][k] * dist[0][k][j] > dist[0][i][j]) {
                        dist[0][i][j] = dist[0][i][k] * dist[0][k][j];
                    }

                    // Case 2: Voucher Path (i->k) + Normal Path (k->j) = Voucher Path (i->j)
                    if (dist[1][i][k] * dist[0][k][j] > dist[1][i][j]) {
                        dist[1][i][j] = dist[1][i][k] * dist[0][k][j];
                    }

                    // Case 3: Normal Path (i->k) + Voucher Path (k->j) = Voucher Path (i->j)
                    if (dist[0][i][k] * dist[1][k][j] > dist[1][i][j]) {
                        dist[1][i][j] = dist[0][i][k] * dist[1][k][j];
                    }
                }
            }
        }

        bool possible = false;
        for (int i = 0; i < n; i++) {
            // Check if loop profit with voucher > 1.0
            if (dist[1][i][i] > 1.0) possible = true;
        }

        if (possible) cout << "Yes (with Golden Voucher)" << endl;
        else cout << "No" << endl;
    }
}

int main() {
    solve();
    return 0;
}