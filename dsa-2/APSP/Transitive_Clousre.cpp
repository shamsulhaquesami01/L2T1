#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <climits>
#include<bitset>
using namespace std;
using ll = long long;
const ll INF = 1e18;
const int MAXN = 505;

void solve(vector<vector<bool> >& reach,int n)
{

    for (int k = 1; k <= n; k++)
    {
        for (int i = 1; i <= n; i++)
        {
            for (int j = 1; j <= n; j++)
            {
                if (reach[i][k] != INF && reach[k][j] != INF)
                {
                    reach[i][j] = reach[i][j]||( reach[i][k] && reach[k][j]);
                }
            }
        }
    }
}
void optimized_solve(vector<bitset<MAXN>>& reach2,int n){
   for (int k = 1; k <= n; k++) {
        for (int i = 1; i <= n; i++) {
            // If i can reach k, then i can also reach everything k can reach.
            if (reach2[i][k]) {
                reach2[i] |= reach2[k]; // <--- This now WORKS and is FAST
            }
        }
    }
}
int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    ll n, m;
    cin >> n >> m ;
    vector<vector<bool> > reach(n + 1, vector<bool>(n + 1, false));

    vector<bitset<MAXN>> reach2(n + 1);


    for (int i = 1; i <= n; i++){
        reach[i][i] = true;
        reach2[i][i]=1;
    }
    for (int i = 0; i < m; i++)
    {
        int a, b;
        ll w;
        cin >> a >> b >> w;
        reach[a][b] = true;
        reach2[a][b] =1;
    }

    solve(reach,n);
    cout << "Transitive Closure Matrix:" << endl;
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            cout << (reach[i][j] ? "1 " : "0 ");
        }
        cout << endl;
    }
    

}

//g++ -std=c++17 dsa-2/APSP/OFFLINE/problem01.cpp -o solution && ./solution < dsa-2/APSP/OFFLINE/input.txt