
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;



void PRIMS_DENSE(int n, vector<pair<int, int>>& coordinates) {
    // dist[i] stores min weight to connect node i to the current tree
    vector<long long> dist(n, LLONG_MAX);
    vector<bool> inMST(n, false);
    
    // Start from node 0
    dist[0] = 0;
    long long total_weight = 0;

    // We iterate N times to add N nodes
    for (int i = 0; i < n; i++) {
        int u = -1;

        // 1. Find the vertex 'u' with minimum dist value among those NOT in MST
        // This linear scan is faster than PQ for dense graphs
        for (int v = 0; v < n; v++) {
            if (!inMST[v] && (u == -1 || dist[v] < dist[u])) {
                u = v;
            }
        }

        // Add u to MST
        inMST[u] = true;
        total_weight += dist[u];

        // 2. Update distance values of adjacent vertices
        // For implicit graphs, "adjacent" means ALL other nodes
        for (int v = 0; v < n; v++) {
            if (!inMST[v]) {
                long long weight = calculateDistance(coordinates[u], coordinates[v]);
                if (weight < dist[v]) {
                    dist[v] = weight;
                }
            }
        }
    }
    cout << total_weight << endl;
}

//----------------------------------

int n;
vector<vector<int>> adj; // adjacency matrix of graph
const int INF = 1000000000; // weight INF means there is no edge

struct Edge {
    int w = INF, to = -1;
};

void prim() {
    int total_weight = 0;
    vector<bool> selected(n, false);
    vector<Edge> min_e(n);
    min_e[0].w = 0;

    for (int i=0; i<n; ++i) {
        int v = -1;
        for (int j = 0; j < n; ++j) {
            if (!selected[j] && (v == -1 || min_e[j].w < min_e[v].w))
                v = j;
        }

        if (min_e[v].w == INF) {
            cout << "No MST!" << endl;
            exit(0);
        }

        selected[v] = true;
        total_weight += min_e[v].w;
        if (min_e[v].to != -1)
            cout << v << " " << min_e[v].to << endl;

        for (int to = 0; to < n; ++to) {
            if (adj[v][to] < min_e[to].w)
                min_e[to] = {adj[v][to], v};
        }
    }

    cout << total_weight << endl;
}