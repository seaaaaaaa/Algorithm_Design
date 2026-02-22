#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    // Optimize standard I/O operations for performance
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    // Read the number of billboards (1 <= N <= 10,000)
    if (!(cin >> n)) return 0;

    vector<int> c(n + 1, 0);
    // Read the customers for each billboard (1 <= c[i] <= 1,000)
    for (int i = 1; i <= n; ++i) {
        cin >> c[i];
    }

    // dp[i] will store the maximum customers considering up to billboard i
    vector<int> dp(n + 1, 0);

    // Base cases
    if (n >= 1) dp[1] = c[1];
    if (n >= 2) dp[2] = max(c[1], c[2]);
    

    // Fill the DP table using the recurrence relation
    for (int i = 3; i <= n; ++i) {
        // We either skip the current billboard i (dp[i-1]) 
        // or we pick it and add it to the best result from i-3 (dp[i-3] + c[i])
        dp[i] = max(dp[i - 1], dp[i - 2] + c[i]);
    }

    // Output the maximum total number of customers
    cout << dp[n] << "\n";

    return 0;
}
