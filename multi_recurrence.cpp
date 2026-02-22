#include <iostream>
#include <vector>

using namespace std;

const int MOD = 32717;

int main() {
    // Optimize standard I/O operations for performance
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, k;
    if (!(cin >> n >> k)) return 0;

    // Create a DP array to store the sequence R(1) to R(n).
    // We use 1-based indexing, so we allocate n + 1 elements.
    vector<int> R(n + 1, 0);

    // Read the base cases a_1 to a_k
    for (int i = 1; i <= k; ++i) {
        cin >> R[i];
        R[i] %= MOD; // Modulo just in case the input is extremely large
    }

    // Calculate R(k+1) to R(n)
    for (int i = k + 1; i <= n; ++i) {
        int p_i;
        cin >> p_i;
        
        long long sum = 0;
        for (int j = 0; j < p_i; ++j) {
            int b_ij;
            cin >> b_ij;
            
            int prev_idx = i - b_ij;
            
            // If the index is <= 0, the value is 0, so we only add if prev_idx > 0
            if (prev_idx > 0) {
                sum = (sum + R[prev_idx]) % MOD;
            }
        }
        R[i] = sum;
    }

    // Print the final sequence R(1) to R(n)
    for (int i = 1; i <= n; ++i) {
        cout << R[i] << (i == n ? "" : " ");
    }
    cout << "\n";

    return 0;
}
