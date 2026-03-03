#include <iostream>
#include <vector>

using namespace std;

/**
 * Problem: Bit String without 101
 * Constraint: String of length n must not contain the substring "101"
 * Modulo: 100,000,007
 */

int main() {
    int n;
    if (!(cin >> n)) return 0; //

    const int MOD = 100000007; //

    // dp[i][state] where state represents the suffix:
    // 0: ends in 00 or is empty
    // 1: ends in 1
    // 2: ends in 10
    vector<vector<long long>> dp(n + 1, vector<long long>(3, 0));

    // Base Case: length 0
    dp[0][0] = 1;

    for (int i = 0; i < n; ++i) {
        // From state 0 (ends in 00 or nothing)
        // Add 0 -> stays in state 0
        dp[i+1][0] = (dp[i+1][0] + dp[i][0]) % MOD;
        // Add 1 -> goes to state 1
        dp[i+1][1] = (dp[i+1][1] + dp[i][0]) % MOD;

        // From state 1 (ends in 1)
        // Add 0 -> goes to state 2 (ends in 10)
        dp[i+1][2] = (dp[i+1][2] + dp[i][1]) % MOD;
        // Add 1 -> stays in state 1 (ends in 11, but for us that's just "ends in 1")
        dp[i+1][1] = (dp[i+1][1] + dp[i][1]) % MOD;

        // From state 2 (ends in 10)
        // Add 0 -> goes to state 0 (ends in 00)
        dp[i+1][0] = (dp[i+1][0] + dp[i][2]) % MOD;
        // Add 1 -> FORBIDDEN (would create 101)
    }

    long long total = (dp[n][0] + dp[n][1] + dp[n][2]) % MOD;
    cout << total << endl;

    return 0;
}
