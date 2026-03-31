#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

const int INF = 1e9; // A large number representing infinity

int main() {
    // Optimize standard I/O operations for faster execution
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, m, k;
    if (!(cin >> n >> m >> k)) return 0;

    vector<int> initial_servers(k);
    for (int i = 0; i < k; ++i) {
        cin >> initial_servers[i];
    }

    vector<int> c(n);
    for (int i = 0; i < n; ++i) {
        cin >> c[i];
    }

    // Build the adjacency list for the network
    vector<vector<int> > adj(n);
    for (int i = 0; i < m; ++i) {
        int u, v;
        cin >> u >> v;
        // Network cables are bidirectional
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    // Distance array tracks the minimum time taken for a server to be hacked
    vector<int> dist(n, INF);
    
    // Min-Priority Queue to process the fastest hacks first. 
    // Stores pairs of (time_to_hack, server_id)
    priority_queue<pair<int, int>, vector<pair<int, int> >, greater<pair<int, int> > > pq;

    // Initialize the multi-source Dijkstra with the K starting servers
    for (int i = 0; i < k; ++i) {
        int start_server = initial_servers[i];
        dist[start_server] = c[start_server];
        pq.push(make_pair(c[start_server], start_server));
    }

    // Run Dijkstra's Algorithm
    while (!pq.empty()) {
        int current_time = pq.top().first;
        int u = pq.top().second;
        pq.pop();

        // If we already found a faster way to hack this server, skip it
        if (current_time > dist[u]) continue;

        // Try to hack all neighbor servers
        for (int i = 0; i < adj[u].size(); ++i) {
            int v = adj[u][i];
            
            // The time to hack neighbor V is the time U finished + V's defense time
            if (dist[u] + c[v] < dist[v]) {
                dist[v] = dist[u] + c[v];
                pq.push(make_pair(dist[v], v));
            }
        }
    }

    // Find the maximum time taken across all servers
    // (This is the time when the entire network is finally compromised)
    int max_time = 0;
    for (int i = 0; i < n; ++i) {
        max_time = max(max_time, dist[i]);
    }

    // Output the final time (Note: the problem description mentions outputting choke points, 
    // but the example clearly shows we output a single integer representing the maximum time)
    cout << max_time << "\n";

    return 0;
}

