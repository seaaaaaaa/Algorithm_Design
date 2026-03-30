#include <iostream>
#include <vector>
#include <climits>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N;
    if (!(cin >> N)) return 0;

    // Build the adjacency matrix
    vector<vector<int>> adj(N, vector<int>(N, 0));
    for (int i = 0; i < N - 1; ++i) {
        for (int j = i + 1; j < N; ++j) {
            int cost;
            cin >> cost;
            adj[i][j] = cost;
            adj[j][i] = cost;
        }
    }

    vector<bool> inMST(N, false);
    vector<int> minCost(N, INT_MAX);
    
    // Start from node 0
    minCost[0] = 0;
    int total_cost = 0;

    for (int count = 0; count < N; ++count) {
        // Pick the minimum cost vertex not yet included in MST
        int u = -1;
        for (int i = 0; i < N; ++i) {
            if (!inMST[i] && (u == -1 || minCost[i] < minCost[u])) {
                u = i;
            }
        }

        // Add the picked vertex to the MST
        inMST[u] = true;
        total_cost += minCost[u];

        // Update the minCost for adjacent vertices
        for (int v = 0; v < N; ++v) {
            // FIX: Removed "adj[u][v] &&" to allow 0-weight edges
            if (!inMST[v] && adj[u][v] < minCost[v]) {
                minCost[v] = adj[u][v];
            }
        }
    }

    // The output must contain exactly one line giving the minimal total cost [cite: 20]
    cout << total_cost << "\n";

    return 0;
}
