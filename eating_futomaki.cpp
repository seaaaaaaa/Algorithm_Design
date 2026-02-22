#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    // Optimize fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    if (!(cin >> n)) return 0;

    vector<int> A(n);
    for (int i = 0; i < n; ++i) {
        cin >> A[i];
    }

    // dp_prev2 stores the results for subarrays of length (len - 2)
    // dp_curr stores the results for subarrays of current length (len)
    vector<int> dp_prev2(n, 0); 
    vector<int> dp_curr(n, 0);

    // Build the DP table for even lengths from 2 up to n
    for (int len = 2; len <= n; len += 2) {
        for (int L = 0; L <= n - len; ++L) {
            int R = L + len - 1;
            
            // Choice 1: Two from the left
            int opt1 = max(A[L], A[L+1]) + (len > 2 ? dp_prev2[L+2] : 0);
            
            // Choice 2: Two from the right
            int opt2 = max(A[R-1], A[R]) + (len > 2 ? dp_prev2[L] : 0);
            
            // Choice 3: One from left, one from right
            int opt3 = max(A[L], A[R]) + (len > 2 ? dp_prev2[L+1] : 0);
            
            // Take the maximum of all three valid moves
            dp_curr[L] = max({opt1, opt2, opt3});
        }
        // Move current state to previous state for the next length iteration
        dp_prev2 = dp_curr;
    }

    // The answer for the full array A[0...n-1]
    cout << dp_curr[0] << "\n";

    return 0;
 }
