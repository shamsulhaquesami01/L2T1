//  Uniqueness of MST
// Concept: Determine if the graph has only one unique Minimum Spanning Tree. Logic:
// Simple Check: If all edge weights are distinct, the MST is unique.
// General Check: Run Kruskal's. 
// If at any point you have multiple edges of the same weight that could 
// connect the same two components, 
// the MST is NOT unique

void solveUniqueness(int n, vector<Edge>& edges) {
    sort(edges.begin(), edges.end());
    DSU dsu(n);
    
    int i = 0;
    bool unique = true;
    
    // Process edges in batches of equal weight
    while (i < edges.size()) {
        int j = i;
        // Find range [i, j) with same weight
        while (j < edges.size() && edges[j].w == edges[i].w) {
            j++;
        }
        
        // Step 1: Count how many edges in this batch COULD connect components
        int useful_edges = 0;
        for (int k = i; k < j; k++) {
            if (dsu.find(edges[k].from) != dsu.find(edges[k].to)) {
                useful_edges++;
            }
        }
        
        // Step 2: Perform Unions
        // Note: We only perform union if they are still disjoint at the moment of processing
        int actual_merges = 0;
        for (int k = i; k < j; k++) {
            if (dsu.find(edges[k].from) != dsu.find(edges[k].to)) {
                dsu.unite(edges[k].from, edges[k].to);
                actual_merges++;
            }
        }
        
        // LOGIC: If we had more potentially useful edges than actual merges we performed,
        // it means we had a choice -> NOT Unique.
        if (useful_edges > actual_merges) {
            unique = false;
            break;
        }
        
        i = j; // Move to next batch
    }
    
    if (unique) cout << "MST is Unique" << endl;
    else cout << "MST is NOT Unique" << endl;
}