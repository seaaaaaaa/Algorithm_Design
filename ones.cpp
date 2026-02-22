#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// Precompute sequence q_k where q_k consists of k ones 
// Maximum n is 10^9, so 10 elements (up to 1,111,111,111) are enough.
vector<long long> q = {
    0, 
    1, 
    11, 
    111, 
    1111, 
    11111, 
    111111, 
    1111111, 
    11111111, 
    111111111, 
    1111111111
};

long long solve(long long n, int k) {
    // Base cases
    if (n == 0) return 0;
    if (k == 1) return n; // Using single '1's costs exactly 'n' digits

    long long q_k = q[k];
    long long c = n / q_k; // equivalent to floor(n / q_k) 
    
    // Option 1: Use q_k exactly 'c' times and solve for the remaining gap
    long long cost1 = c * k + solve(n - c * q_k, k - 1);
    
    // Option 2: Use q_k exactly 'c + 1' times and solve for the overshoot gap
    long long cost2 = (c + 1) * k + solve((c + 1) * q_k - n, k - 1);
    
    // Return the minimal number of digits 1 [cite: 26, 30]
    return min(cost1, cost2);
}

int main() {
    // Optimize standard I/O operations for performance
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    long long n;
    // Read the input n (1 <= n < 10^9) 
    if (cin >> n) {
        // Start checking from the largest precomputed repunit length (k = 10)
        cout << solve(n, 10) << "\n";
    }
    
    return 0;
}
