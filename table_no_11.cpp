#include <iostream>

using namespace std;

int main() {
    // Optimize standard I/O operations for performance
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    long long n;
    // Check if valid input is received
    if (!(cin >> n)) return 0;

    // Modulo specified in the requirements
    long long MOD = 100000007;

    // Base cases for n = 1
    long long A = 1; // Number of ways to build the column ending in (0, 0)
    long long B = 1; // Number of ways to build the column ending in (0, 1) or (1, 0)

    // Calculate dynamically for columns 2 through n
    for (int i = 2; i <= n; ++i) {
        long long next_A = (A + 2 * B) % MOD;
        long long next_B = (A + B) % MOD;
        
        A = next_A;
        B = next_B;
    }

    // The total valid configurations is the sum of all state variations
    long long total_ways = (A + 2 * B) % MOD;

    // Output the result
    cout << total_ways << "\n";

    return 0;
}
