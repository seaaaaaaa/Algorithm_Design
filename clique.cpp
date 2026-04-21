#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

int N;
long long B[55];
long long adj[55];
long long max_power = 0;

void solve(long long candidates, long long current_power) {
    // Update global max
    if (current_power > max_power) {
        max_power = current_power;
    }

    if (candidates == 0) return;

    // --- Aggressive Pruning ---
    long long potential_remaining = 0;
    long long temp_c = candidates;
    while (temp_c > 0) {
        int i = __builtin_ctzll(temp_c);
        potential_remaining += B[i];
        temp_c &= ~(1LL << i);
    }

    if (current_power + potential_remaining <= max_power) {
        return;
    }
    // ---------------------------

    while (candidates > 0) {
        // Optimization: if the current best possible can't be beat even 
        // with the best remaining person, we can technically stop here too.
        
        int v = __builtin_ctzll(candidates); 
        
        // Only recurse if there's a chance to add more members
        long long next_candidates = candidates & adj[v];
        
        // Remove v from next_candidates to ensure the bitmask always shrinks
        next_candidates &= ~(1LL << v); 
        
        solve(next_candidates, current_power + B[v]);

        // Backtrack: remove v from the current pool
        candidates &= ~(1LL << v);
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    if (!(cin >> N)) return 0;

    for (int i = 0; i < N; i++) cin >> B[i];

    for (int i = 0; i < N; i++) {
        string row;
        cin >> row;
        adj[i] = 0;
        for (int j = 0; j < N; j++) {
            // Only add other friends; we handle v separately in the logic now
            if (row[j] == '1') {
                adj[i] |= (1LL << j);
            }
        }
    }

    long long all_start = (N == 64) ? -1LL : (1LL << N) - 1;

    solve(all_start, 0);

    cout << max_power << endl;

    return 0;
}
