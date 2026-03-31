#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// Structure to represent a Mass Relay (Edge)
struct Edge {
    int u, v, w;
    
    // This tells C++ how to sort the edges (smallest weight first)
    bool operator<(const Edge& other) const {
        return w < other.w;
    }
};

// Disjoint Set Union (DSU) / Union-Find structure
// This efficiently keeps track of which planets are in which clusters
struct DSU {
    vector<int> parent;
    
    // Initialize N isolated clusters
    DSU(int n) {
        parent.resize(n);
        for (int i = 0; i < n; i++) {
            parent[i] = i; 
        }
    }
    
    // Find the "leader" of a planet's cluster (with path compression for speed)
    int find(int i) {
        if (parent[i] == i)
            return i;
        return parent[i] = find(parent[i]);
    }
    
    // Merge two clusters. Returns true if they were successfully merged, 
    // false if they were already in the same cluster.
    bool unite(int i, int j) {
        int root_i = find(i);
        int root_j = find(j);
        
        if (root_i != root_j) {
            parent[root_i] = root_j;
            return true;
        }
        return false;
    }
};

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, m, q;
    if (!(cin >> n >> m >> q)) return 0;

    vector<Edge> edges(m);
    for (int i = 0; i < m; i++) {
        cin >> edges[i].u >> edges[i].v >> edges[i].w;
    }

    // 1. Sort all mass relays from shortest to longest distance
    sort(edges.begin(), edges.end());

    // 2. Initialize DSU and the answer array
    DSU dsu(n);
    vector<int> ans(n + 1, 0);
    int current_clusters = n;

    // 3. Process the edges (Kruskal's Algorithm)
    for (int i = 0; i < m; i++) {
        // If the two planets are in different clusters, merge them!
        if (dsu.unite(edges[i].u, edges[i].v)) {
            current_clusters--; // We successfully merged, so we have 1 less cluster
            
            // The range K required to reach this number of clusters 
            // is exactly the distance of the relay we just built!
            ans[current_clusters] = edges[i].w;
            
            // If the whole galaxy is connected into 1 cluster, we can stop early
            if (current_clusters == 1) {
                break;
            }
        }
    }

    // 4. Answer the Q queries instantly in O(1) time
    for (int i = 0; i < q; i++) {
        int D;
        cin >> D;
        
        // If they ask for more clusters than the total number of planets, K is 0
        if (D >= n) {
            cout << 0 << "\n";
        } else {
            cout << ans[D] << "\n";
        }
    }

    return 0;
}
