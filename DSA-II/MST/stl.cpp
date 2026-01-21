/**
 * =========================================================
 * C++ STL & IMPLEMENTATION MASTER REFERENCE
 * Author: Gemini (For Lab Test Prep)
 * * *CONTENTS:
 * 1.  PAIRS & TUPLES (Handling coordinates, edges)
 * 2.  SORTING MASTERY (Lambdas, Structs, Input Order)
 * 3.  PRIORITY QUEUES (Min-Heap, Max-Heap, Custom)
 * 4.  MAPS & SETS (String mapping, Frequency counting)
 * 5.  VECTORS & GRIDS (Adjacency lists, Resizing)
 * 6.  COORDINATE COMPRESSION (Handling large coords)
 * 7.  USEFUL ALGORITHMS (Unique, Lower_bound)
 * =========================================================
 */

#include <iostream>
#include <vector>
#include <algorithm> // sort, min, max, unique, lower_bound
#include <map>
#include <unordered_map>
#include <set>
#include <queue>     // priority_queue
#include <stack>
#include <tuple>     // for tuple<int, int, int>
#include <string>
#include <climits>   // INT_MAX, LLONG_MAX

using namespace std;

// =========================================================
// 1. PAIRS & TUPLES
// =========================================================
void section_pairs_tuples() {
    // --- PAIRS (2 elements) ---
    // Great for: {node, weight}, {x, y}
    pair<int, int> p1 = {10, 20};
    
    // Accessing
    int x = p1.first;
    int y = p1.second;
    
    // Nested Pairs (Triples) - e.g., {weight, {u, v}}
    pair<int, pair<int, int>> edge = {5, {1, 2}};
    int w = edge.first;
    int u = edge.second.first;
    
    // Comparison: Pairs compare .first, then .second automatically
    pair<int, int> a = {1, 5};
    pair<int, int> b = {1, 2};
    // (a < b) is FALSE because 1==1, but 5 > 2.
    
    // --- TUPLES (3+ elements) ---
    // Great for: {u, v, w} or {x, y, z} without defining a struct
    tuple<int, int, int> t1 = {1, 2, 5};
    
    // Accessing (Syntax is weird, use get<index>)
    int node_u = get<0>(t1);
    int node_v = get<1>(t1);
    int weight = get<2>(t1);
    
    // Tie: Unpacking a tuple/pair quickly
    int A, B, C;
    tie(A, B, C) = t1; // A=1, B=2, C=5
}

// =========================================================
// 2. SORTING MASTERY (CRITICAL FOR MST)
// =========================================================

// --- A. The Struct Approach (Best for Edges) ---
struct Edge {
    int u, v;
    long long w;
    int id; // To save original input index
    
    // Operator Overloading: Defines default sort order
    // "const" at the end is mandatory
    bool operator<(const Edge& other) const {
        // Example: Sort by Weight (Ascending)
        if (w != other.w) return w < other.w;
        return id < other.id; // Tie-breaker: original index
    }
};

// --- B. The Custom Comparator Function ---
bool compareDescending(const int& a, const int& b) {
    return a > b; // 10, 9, 8...
}

void section_sorting() {
    vector<int> v = {1, 5, 2, 8};
    
    // 1. Standard Sort (Ascending)
    sort(v.begin(), v.end()); 
    
    // 2. Reverse Sort (Descending)
    sort(v.rbegin(), v.rend());
    
    // 3. Sort Pairs (Default: First element, then Second)
    vector<pair<int, int>> coords = {{2, 5}, {1, 10}, {2, 1}};
    sort(coords.begin(), coords.end()); 
    // Result: {1, 10}, {2, 1}, {2, 5}
    
    // 4. PRESERVING INPUT ORDER (The "Section C" Requirement)
    // Problem: Sort edges by weight, but if equal, keep original relative order?
    // Or just need to know which original edge index corresponds to the MST edge.
    vector<Edge> edges;
    edges.push_back({1, 2, 10, 0}); // 0 is original index
    edges.push_back({2, 3, 5, 1});  // 1 is original index
    
    sort(edges.begin(), edges.end()); // Uses the operator< inside struct
    
    // 5. LAMBDA SORTING (Modern C++ - Very Fast to Write)
    // Useful for sorting simple vectors without writing a separate function
    vector<pair<int, int>> points = {{1, 2}, {3, 1}, {5, 10}};
    
    // Example: Sort by Y coordinate only
    sort(points.begin(), points.end(), [](const pair<int,int>& a, const pair<int,int>& b) {
        return a.second < b.second; 
    });
}

// =========================================================
// 3. PRIORITY QUEUES (FOR PRIM'S / DIJKSTRA)
// =========================================================
void section_priority_queues() {
    // 1. Max-Heap (Default) - Pops LARGEST element first
    priority_queue<int> max_pq;
    max_pq.push(10); max_pq.push(5);
    // max_pq.top() is 10
    
    // 2. Min-Heap (CRITICAL FOR MST/PRIM'S)
    // Syntax: <Type, Container, Comparator>
    priority_queue<int, vector<int>, greater<int>> min_pq;
    min_pq.push(10); min_pq.push(5);
    // min_pq.top() is 5
    
    // 3. Priority Queue of Pairs
    // Max-Heap of pairs sorts by pair.first (largest first)
    priority_queue<pair<int, int>> pq_pairs; 
    
    // Min-Heap of pairs (Smallest .first pops first)
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> min_pq_pairs;
    // Useful for Prim's: push({weight, node}) -> pops cheapest edge automatically
    
    // 4. Priority Queue of Custom Structs
    struct Node {
        int id;
        int dist;
        // For PQ, you usually need to reverse the logic
        // Because PQ puts "largest" at top. If you want smallest dist at top,
        // you must say "I am smaller than other if my dist is GREATER".
        bool operator<(const Node& other) const {
            return dist > other.dist; // This creates a MIN-HEAP behavior for 'dist'
        }
    };
    priority_queue<Node> struct_pq;
}

// =========================================================
// 4. MAPS, SETS & STRINGS (HANDLING NON-INTEGER NODES)
// =========================================================
void section_maps_sets() {
    // Scenario: Input gives "Dhaka", "Chittagong" instead of 1, 2.
    // We need to map strings to ID 0..N-1 for DSU/Arrays.
    
    map<string, int> city_to_id;
    int id_counter = 0;
    string s = "Dhaka";
    
    // Checking existence
    if (city_to_id.find(s) == city_to_id.end()) {
        city_to_id[s] = id_counter++; // Assign new ID
    }
    int u = city_to_id[s]; // Get ID
    
    // Reverse Mapping (ID to String) - For printing output
    vector<string> id_to_city(100); 
    id_to_city[u] = s;
    
    // --- SETS (Unique Sorted Elements) ---
    set<int> distinct_weights;
    distinct_weights.insert(10);
    distinct_weights.insert(10); // Ignored
    // distinct_weights contains {10}
    
    // Check if element exists
    if (distinct_weights.count(5)) { /* exists */ }
}

// =========================================================
// 5. COORDINATE COMPRESSION (TRICK FOR HUGE COORDINATES)
// =========================================================
// Scenario: Coordinates are 0 to 10^9. You can't make an array arr[1000000000].
// But N is only 1000. You only care about relative order (1st smallest, 2nd smallest...).
void section_coordinate_compression() {
    vector<long long> raw_coords = {1000000000, 5, 5, 20};
    vector<long long> temp = raw_coords;
    
    // 1. Sort
    sort(temp.begin(), temp.end());
    // 2. Remove Duplicates
    // unique() moves duplicates to end, erase() cuts them off
    temp.erase(unique(temp.begin(), temp.end()), temp.end());
    // temp is now: {5, 20, 1000000000}
    
    // 3. Map original values to 0, 1, 2...
    // lower_bound returns iterator to the first element >= val
    // subtracting .begin() gives the INDEX
    for(long long x : raw_coords) {
        int compressed_id = lower_bound(temp.begin(), temp.end(), x) - temp.begin();
        // 1000000000 -> becomes index 2
        // 5 -> becomes index 0
    }
}

// =========================================================
// 6. THE "SECTION C" SPECIAL (GRID/GEOMETRY TRICK)
// =========================================================
// How to sort points 3 times (X, Y, Z) and track IDs
struct Point {
    int id;
    long long x, y, z;
};

void section_geometry_mst() {
    int n = 5;
    vector<Point> pts(n);
    // ... input ...
    
    vector<Edge> edges; // The edge struct from Section 2
    
    // Step 1: Sort by X
    sort(pts.begin(), pts.end(), [](Point a, Point b) { return a.x < b.x; });
    for(int i=0; i<n-1; i++) {
        // Add edge between adjacent points in X-sorted order
        // Cost = difference in X
        edges.push_back({pts[i].id, pts[i+1].id, abs(pts[i].x - pts[i+1].x), -1});
    }
    
    // Step 2: Sort by Y (Re-sort the SAME vector)
    sort(pts.begin(), pts.end(), [](Point a, Point b) { return a.y < b.y; });
    for(int i=0; i<n-1; i++) {
        edges.push_back({pts[i].id, pts[i+1].id, abs(pts[i].y - pts[i+1].y), -1});
    }
    
    // Now you have ~2N edges instead of N^2. Safe to run Kruskal's.
}

// =========================================================
// 7. COMPLETE PRIM'S ALGORITHM TEMPLATE (DENSE GRAPHS)
// =========================================================
// Use this if N <= 2000 and M is close to N*N (Adjacency Matrix given)
void prims_algorithm(int n, vector<vector<pair<int, int>>>& adj) {
    // Min-Heap: {weight, node}
    priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> pq;
    
    vector<int> dist(n+1, INT_MAX);
    vector<bool> inMST(n+1, false);
    vector<int> parent(n+1, -1); // To reconstruct the tree
    
    int start_node = 1;
    dist[start_node] = 0;
    pq.push({0, start_node});
    
    long long mst_cost = 0;
    
    while(!pq.empty()) {
        int u = pq.top().second;
        int d = pq.top().first;
        pq.pop();
        
        if(inMST[u]) continue; // Already processed
        inMST[u] = true;
        mst_cost += d;
        
        for(auto& edge : adj[u]) {
            int v = edge.first;
            int w = edge.second;
            if(!inMST[v] && w < dist[v]) {
                dist[v] = w;
                pq.push({dist[v], v});
                parent[v] = u;
            }
        }
    }
    cout << mst_cost << endl;
}

// =========================================================
// MAIN (For Copy-Paste Testing)
// =========================================================
int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    // Floating point precision (if needed)
    // cout << fixed << setprecision(6) << ans << endl;
    
    return 0;
}