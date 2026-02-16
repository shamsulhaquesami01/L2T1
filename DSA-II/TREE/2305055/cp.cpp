#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

using ll =long long;

struct funtion {
    ll a, b, c;
    int id;
};

bool is_above(const funtion& f, const funtion& g) {
    ll A = f.a - g.a;
    ll B = f.b - g.b;
    ll C = f.c - g.c;

    if (A > 0) {
        // b square - 4ac 
        return B * B < 4 * A * C;
    } else if (A == 0) {
        return B == 0 && C > 0;
    } else {
        // down hoile minus inf e jabe 
        return false;
    }
}

int memo_down[3005];
int memo_up[3005];
vector<int> adj[3005];      
vector<int> rev_adj[3005];  

int downHighest(int u) {
    if (memo_down[u] != -1) return memo_down[u];
    int highest_len = 0;
    for (int v : adj[u]) {
        highest_len = max(highest_len, downHighest(v));
    }
    return memo_down[u] = 1 + highest_len;
}

int uphighest(int u) {
    if (memo_up[u] != -1) return memo_up[u];
    int highest_len = 0;
    for (int v : rev_adj[u]) {
        highest_len = max(highest_len, uphighest(v));
    }
    return memo_up[u] = 1 + highest_len;
}

void solve() {
    int n;
    cin >> n;
    vector<funtion> funtions(n);
    for (int i = 0; i < n; ++i) {
        cin >> funtions[i].a >> funtions[i].b >> funtions[i].c;
        funtions[i].id = i;
    }

    //reset kora lagbe protibar
    for(int i=0; i<n; ++i) {
        adj[i].clear();
        rev_adj[i].clear();
        memo_down[i] = -1;
        memo_up[i] = -1;
    }

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i == j) continue;
            if (is_above(funtions[i], funtions[j])) {
                adj[i].push_back(j);
                rev_adj[j].push_back(i);
            }
        }
    }

    for (int i = 0; i < n; ++i) {
        int ans = uphighest(i) + downHighest(i) - 1;
        cout << ans << (i == n - 1 ? "" : " ");
    }
    cout << "\n";
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int t;
    cin >> t;
    while (t--) {
        solve();
    }
    return 0;
}