#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

const int MAX_CITIES = 26;

// Function to compute transitive closure
void computeClosure(vector<vector<bool>>& reach) {
    for (int k = 0; k < MAX_CITIES; k++) {
        for (int i = 0; i < MAX_CITIES; i++) {
            // Optimization: If i cannot reach k, no need to check j
            if (reach[i][k]) {
                for (int j = 0; j < MAX_CITIES; j++) {
                    if (reach[k][j]) {
                        reach[i][j] = true;
                    }
                }
            }
        }
    }
}

void solve(bool firstCase) {
    int n;
    cin >> n;

    vector<vector<bool>> mat1(MAX_CITIES, vector<bool>(MAX_CITIES, false));
    vector<vector<bool>> mat2(MAX_CITIES, vector<bool>(MAX_CITIES, false));

    // Initialize diagonals (can always reach self)
    for (int i = 0; i < MAX_CITIES; i++) {
        mat1[i][i] = true;
        mat2[i][i] = true;
    }

    // Read Airline 1
    for (int i = 0; i < n; i++) {
        char u, v;
        cin >> u >> v;
        mat1[u - 'A'][v - 'A'] = true;
    }

    int m;
    cin >> m;
    // Read Airline 2
    for (int i = 0; i < m; i++) {
        char u, v;
        cin >> u >> v;
        mat2[u - 'A'][v - 'A'] = true;
    }

    // Compute Reachability for both
    computeClosure(mat1);
    computeClosure(mat2);

    // Compare
    bool equivalent = true;
    for (int i = 0; i < MAX_CITIES; i++) {
        for (int j = 0; j < MAX_CITIES; j++) {
            if (mat1[i][j] != mat2[i][j]) {
                equivalent = false;
                break;
            }
        }
        if (!equivalent) break;
    }

    if (!firstCase) cout << endl; // Blank line between outputs
    if (equivalent) cout << "YES" << endl;
    else cout << "NO" << endl;
}

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int t;
    cin >> t;
    bool first = true;
    while (t--) {
        solve(first);
        first = false;
    }
    return 0;
}