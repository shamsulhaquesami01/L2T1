
// Once upon a time there lived a king in a far far country. In the country, there are n cities and m roads.
//  He was severely attacked by his enemy. The enemy damaged all the cities of King's country.
//   As the roads between the cities had been damaged, the King wanted to repair those. So 
//   he decided to launch a tender for this.As King's country is a well managed country. By well managed country,
//    we mean that it is possible to go from each city to any other city. 
//    But now as the city has been destroyed by enemies, all the roads are broken, 
//    the king will like to rebuild the roads in such a way that it is still a well managed country.
// Cost of repairing the road in the country is really weird, it is not addition of costs but it is
//  instead multiplication of those. What it means that if the king decides that he should repair 5 roads, 
//  then total cost of repairing those roads will be multiplication of all the 5 costs.
// As the King's treasure has been damaged by the attack of foreign city, he would like to spend minimum 
// amount of money and that the will want that his country still remains well managed country. Surprisingly
//  the company that was given tender had a rule that all the costs will be in powers of two, as they were 
//  really love with binary numbers.
// As value of the total cost can be really large. We do not want to know the actual cost, instead output 
// number of divisors of the number.
// Input
// T: number of test cases (T ≤ 5)
// n and m (n ≤ 105, m ≤ 2 × 105)
// Next m lines will have a, b, c, which denotes that cities a and b are connected with road of cost c.
// (2 ≤ c ≤ 1018, and c will always be power of 2) (1 ≤ a ≤ n) (1 ≤ b ≤ n)
// Output
// For each test case, output a single line containing a number as stated in the problem.







#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

// Structure to represent an edge
struct Edge {
    int u, v;
    long long exponent; // We store the exponent 'x' where Cost = 2^x

    // Sort edges based on the exponent (Standard MST logic)
    bool operator<(const Edge& other) const {
        return exponent < other.exponent;
    }
};

// Standard DSU (Disjoint Set Union) for Kruskal's Algorithm
class DSU {
    vector<int> parent;
public:
    DSU(int n) {
        parent.resize(n + 1);
        for (int i = 0; i <= n; i++) parent[i] = i;
    }

    int find(int i) {
        if (parent[i] == i) return i;
        return parent[i] = find(parent[i]);
    }

    void unite(int i, int j) {
        int root_i = find(i);
        int root_j = find(j);
        if (root_i != root_j) {
            parent[root_i] = root_j;
        }
    }
};

void solve() {
    int n, m;
    if (!(cin >> n >> m)) return;

    vector<Edge> edges;
    for (int i = 0; i < m; i++) {
        int u, v;
        unsigned long long c; // Cost can be up to 10^18
        cin >> u >> v >> c;

        // Convert the cost 'c' (power of 2) into its exponent.
        // E.g., if c = 8 (2^3), exponent is 3.
        // Since c is guaranteed to be a power of 2, we can count trailing zeros
        // or loop. __builtin_ctzll is efficient for GCC/Clang.
        long long exponent = 0;
        while (c > 1) {
            c >>= 1;
            exponent++;
        }

        edges.push_back({u, v, exponent});
    }

    // 1. Sort edges by exponent
    sort(edges.begin(), edges.end());

    // 2. Run Kruskal's
    DSU dsu(n);
    long long sum_exponents = 0;
    int edges_count = 0;

    for (const auto& edge : edges) {
        if (dsu.find(edge.u) != dsu.find(edge.v)) {
            dsu.unite(edge.u, edge.v);
            sum_exponents += edge.exponent;
            edges_count++;
        }
    }

    // 3. The minimum cost is 2^(sum_exponents).
    // The number of divisors of 2^S is S + 1.
    cout << sum_exponents + 1 << endl;
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