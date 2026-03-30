#include <iostream>
#include <vector>
#include <queue>

using namespace std;

int main() {
    // Optimize standard I/O operations
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, e, k;
    if (!(cin >> n >> e >> k)) return 0;

    // Create an adjacency list for the graph.
    // There are n possible locations plus the base (node 0), so size is n + 1.
    vector<vector<int>> adj(n + 1);
    
    // Read the roads
    for (int i = 0; i < e; ++i) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    // Distance array to keep track of the shortest distance from base (node 0).
    // Initialized to -1 to represent unvisited locations.
    vector<int> dist(n + 1, -1);
    queue<int> q;

    // Start BFS from the base
    dist[0] = 0;
    q.push(0);

    while (!q.empty()) {
        int current = q.front();
        q.pop();

        // If the current distance is k, its neighbors will be at k+1, 
        // which we don't strictly need to explore for this problem.
        if (dist[current] == k) continue; 

        // Explore all connected locations
        for (int neighbor : adj[current]) {
            // If the neighbor hasn't been visited yet
            if (dist[neighbor] == -1) {
                dist[neighbor] = dist[current] + 1;
                q.push(neighbor);
            }
        }
    }

    // Count how many locations have a distance of exactly k
    int outpost_count = 0;
    for (int i = 1; i <= n; ++i) {
        if (dist[i] == k) {
            outpost_count++;
        }
    }

    // Output the result
    cout << outpost_count << "\n";

    return 0;
}
