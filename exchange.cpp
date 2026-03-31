#include <iostream>
#include <vector>

using namespace std;

// Structure to hold our directed edges (currency exchanges)
struct Edge {
    int u, v;
    double weight;
};

void solve() {
    int N;
    cin >> N;
    
    vector<Edge> edges;
    
    // Read the exchange rates and build the edge list
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            double rate;
            cin >> rate;
            // Only add valid exchange rates
            if (rate > 0) {
                // NO LOGARITHM HERE - store the raw exchange rate
                edges.push_back({i, j, rate}); 
            }
        }
    }

    // dist array tracks the maximum money to reach each currency
    // We start with 1.0, implying we start with 1 unit of every currency
    vector<double> dist(N, 1.0);
    bool possible = false;

    // Bellman-Ford Relaxation: Run N - 1 times
    for (int step = 1; step < N; ++step) {
        bool updated = false;
        for (const auto& edge : edges) {
            // MULTIPLICATION CHECK: 
            // If our current money * exchange rate > money we previously recorded for currency V
            if (dist[edge.u] * edge.weight > dist[edge.v] + 1e-8) {
                dist[edge.v] = dist[edge.u] * edge.weight;
                updated = true;
            }
        }
        // Early stopping: if nothing changed in this full pass, no cycle can exist
        if (!updated) break;
    }

    // The N-th pass: Positive Cycle Check
    // If we can STILL increase our profit after N-1 steps, a positive cycle exists
    for (const auto& edge : edges) {
        if (dist[edge.u] * edge.weight > dist[edge.v] + 1e-8) {
            possible = true;
            break;
        }
    }

    // Output result for this testcase
    if (possible) {
        cout << "YES\n";
    } else {
        cout << "NO\n";
    }
}

int main() {
    // Optimize standard I/O operations for faster execution
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int K;
    if (cin >> K) {
        while (K--) {
            solve();
        }
    }
    return 0;
}
