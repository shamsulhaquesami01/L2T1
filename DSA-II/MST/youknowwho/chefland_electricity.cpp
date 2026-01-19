// // There are n villages in a Chefland. Some of the villages have electricity facilities, other doesn't. 
// You can consider the villages arranged in line in the order 1 to n from left to right. i-th of village can be considered at xi coordinates.
// // Chef decided that electricity should be provided to all the villages.
//  So, he decides to buy some amount of electric wires to connect the villeges without electricity to some villages with
//   electricity. As Chef does not want to spend too much amount of money on wires, 
//   can you find out minimum amount of length of wire Chef should buy.
// Input
// First line of the input contains an integer T denoting the number of test cases. T test cases follow.
// First line of each test case contains an integer n denoting number of villages in Chefland.

// Second line will contain a string of length n containing '0' or '1's only. If i-th character of the string is '1', then it denotes that i-th village has electricity.

// Next line contains n space separated integers denoting the x coordinates of the villages in the order from village 1 to n

// Output
// For each test case, output a single line containing a integer corresponding to the minimum length of wire Chef needs to buy.
// Constraints
// 1 ≤ T ≤ 10
// It is guaranteed that there will be at least one village which will have electricity.
// 1 ≤ x1 < x2 < ... < xn ≤ 109
// Subtasks
// Subtask #1 : 30 points
// 1 ≤ N ≤ 1000
// Subtask #2 : 70 points

// 1 ≤ N ≤ 105
// Sample 1:
// Input
// Output
// 2
// 2
// 01
// 1 2
// 3
// 100
// 1 5 6
// 1
// 5
// Explanation:
// In the first example, first village does not have electricity. If we put a wire between village 1 and 2 of length 1, then both the villages will have electricity.

// In the second example, We can a draw a wire from first village to third village, passing through second village.
//  Its total length will be 5. Now all the villages will have electricity. This is the minimum length of wire you will require.

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

void solve() {
    int n;
    if (!(cin >> n)) return;
    
    string s;
    cin >> s;
    
    vector<long long> x(n);
    for (int i = 0; i < n; ++i) {
        cin >> x[i];
    }

    // Store the indices of villages that have electricity
    vector<int> ones;
    for (int i = 0; i < n; ++i) {
        if (s[i] == '1') {
            ones.push_back(i);
        }
    }

    long long total_wire = 0;
    int first_one = ones.front();
    int last_one = ones.back();

    // 1. Cost for leading zeros (connect 1st village to 1st electrified village)
    if (first_one > 0) {
        total_wire += (x[first_one] - x[0]);
    }

    // 2. Cost for trailing zeros (connect last electrified village to last village)
    if (last_one < n - 1) {
        total_wire += (x[n - 1] - x[last_one]);
    }

    // 3. Cost for intervals between consecutive electrified villages
    for (size_t k = 0; k < ones.size() - 1; ++k) {
        int u = ones[k];
        int v = ones[k+1];
        
        // Calculate the maximum gap between adjacent villages in this segment
        long long max_gap = 0;
        for (int i = u; i < v; ++i) {
            long long gap = x[i+1] - x[i];
            if (gap > max_gap) {
                max_gap = gap;
            }
        }
        
        // Total distance covered in this segment
        long long segment_dist = x[v] - x[u];
        
        // We cover the whole segment minus the largest gap
        total_wire += (segment_dist - max_gap);
    }

    cout << total_wire << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int t;
    if (cin >> t) {
        while (t--) {
            solve();
        }
    }
    return 0;
}