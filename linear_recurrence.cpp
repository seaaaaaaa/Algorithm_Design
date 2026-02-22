#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// The problem specifies all answers must be modulo 32717
const int MOD = 32717;

// Helper function to handle negative modulo correctly in C++
long long safe_mod(long long x) {
    return (x % MOD + MOD) % MOD;
}

int main() {
    // Optimize standard I/O operations for performance
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int k, n;
    // Read k and n from the first line
    if (!(cin >> k >> n)) return 0;

    // Read the array of coefficients c_1 to c_k from the second line
    vector<long long> c(k);
    for (int i = 0; i < k; ++i) {
        cin >> c[i];
        c[i] = safe_mod(c[i]);
    }

    // Allocate DP array of size max(k, n + 1) to prevent out-of-bounds
    vector<long long> R(max(k, n + 1));
    
    // Read the base cases a_0 to a_{k-1} from the third line
    for (int i = 0; i < k; ++i) {
        cin >> R[i];
        R[i] = safe_mod(R[i]);
    }

    // Calculate R[i] iteratively up to n
    for (int i = k; i <= n; ++i) {
        long long sum = 0;
        // Apply the linear recurrence formula
        for (int j = 1; j <= k; ++j) {
            // R(i) += c_j * R(i - j)
            sum = safe_mod(sum + safe_mod(c[j - 1] * R[i - j]));
        }
        R[i] = sum;
    }

    // Print the requested value R[n] mod 32717
    cout << R[n] << "\n";

    return 0;
}
