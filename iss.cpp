#include <iostream>
#include <vector>

using namespace std;

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    if (!(cin >> n)) return 0;

    // dp[i] will store the number of increasing sequence sums for the number i
    // Using long long to prevent any potential overflow, though it fits in 32-bit int for N=100
    vector<long long> dp(n + 1, 0);
    
    // Base case: There is 1 way to make the sum 0 (by choosing nothing)
    dp[0] = 1;

    // Iterate through all possible sequence elements from 1 to N
    // This outer loop guarantees the condition ai <= a(i+1) is met 
    for (int i = 1; i <= n; i++) {
        // Update the ways to form sums from 'i' up to 'N'
        for (int j = i; j <= n; j++) {
            dp[j] += dp[j - i];
        }
    }

    // Output exactly one line that gives the total number of distinct sums [cite: 10]
    cout << dp[n] << "\n";

    return 0;
}
