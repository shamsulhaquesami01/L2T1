/*

There are 
N
N cities and initially 
0
0 roads between them. You are the Civil Engineer (but a programmer from heart!) who is in charge of constructing roads between these cities.
The cities are numbered from 
0
0 to 
N
−
1
N−1. There are 
M
M types of road constructions which can be performed among these cities. For each 
k
=
1
,
2
,
3
,
…
,
M
k=1,2,3,…,M, the 
k
k-th kind of road construction is to choose an integer 
x
x such that 
0
≤
x
<
N
0≤x<N and construct a new two-way road connecting city 
x
x and city 
(
x
+
A
k
)
 mod 
N
(x+A 
k
​	
 ) mod N. Performing the 
k
k-th kind of road construction once costs 
C
k
C 
k
​	
  taka.
You can do these 
M
M kinds of road constructions any number of times (possibly zero) in any order. For example, if three kinds of road constructions are available, you can choose to perform the first type of road construction twice, the second zero times, and the third once.
The people of these cities want to travel from one city to another city. You have to determine whether it is possible to make all the cities connected together. If possible, then determine the minimum total cost. Here, “connected” means, there exists at least one path from each city to every other city.
Constraints:
2
≤
N
≤
1
0
9
2≤N≤10 
9
 
1
≤
M
≤
1
0
5
1≤M≤10 
5
 
1
≤
A
k
≤
N
−
1
1≤A 
k
​	
 ≤N−1
1
≤
C
k
≤
1
0
9
1≤C 
k
​	
 ≤10 
9
 
All values in input are integers

*/



#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric> // For std::gcd in C++17, or use custom function

using namespace std;

// Custom GCD function to ensure compatibility with older C++ standards
long long gcd(long long a, long long b) {
    while (b) {
        a %= b;
        swap(a, b);
    }
    return a;
}

struct Construction {
    long long a; // Stride
    long long c; // Cost
};

void solve() {
    long long n;
    int m;
    if (!(cin >> n >> m)) return;

    vector<Construction> ops(m);
    for (int i = 0; i < m; ++i) {
        cin >> ops[i].a >> ops[i].c;
    }

    // 1. Sort road types by cost (Cheapest first - Kruskal's Greedy)
    sort(ops.begin(), ops.end(), [](const Construction& x, const Construction& y) {
        return x.c < y.c;
    });

    long long current_gcd = n; // Initially, N separate components
    long long total_cost = 0;

    for (const auto& op : ops) {
        // If already fully connected (1 component), stop
        if (current_gcd == 1) break;

        // Calculate what the GCD would be if we use this road type
        long long next_gcd = gcd(current_gcd, op.a);

        // If this road type reduces the number of components
        if (next_gcd < current_gcd) {
            // The number of edges needed is the reduction in components
            long long edges_needed = current_gcd - next_gcd;
            
            total_cost += edges_needed * op.c;
            current_gcd = next_gcd;
        }
    }

    // 2. Check if fully connected
    if (current_gcd == 1) {
        cout << total_cost << endl;
    } else {
        cout << -1 << endl;
    }
}

int main() {
    // Fast I/O
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    solve();

    return 0;
}