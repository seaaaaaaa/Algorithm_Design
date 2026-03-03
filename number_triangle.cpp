#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

/**
 * Problem: Number Triangle [cite: 25]
 * Goal: Find the maximum path sum from top to bottom [cite: 35]
 */

int main() {
    int N;
    if (!(cin >> N)) return 0; // N is the number of levels (1 <= N <= 100) [cite: 43]

    // We use a 2D vector to store the triangle values
    vector<vector<int>> triangle(N, vector<int>(N));
    vector<vector<int>> dp(N, vector<int>(N, 0));

    // Input the triangle numbers [cite: 43, 45]
    for (int i = 0; i < N; i++) {
        for (int j = 0; j <= i; j++) {
            cin >> triangle[i][j];
        }
    }

    // Initialize the top of the DP table 
    dp[0][0] = triangle[0][0];

    // Build the DP table level by level [cite: 39]
    for (int i = 1; i < N; i++) {
        for (int j = 0; j <= i; j++) {
            if (j == 0) {
                // Far left edge: can only come from directly above
                dp[i][j] = dp[i-1][j] + triangle[i][j];
            } else if (j == i) {
                // Far right edge: can only come from diagonally left above
                dp[i][j] = dp[i-1][j-1] + triangle[i][j];
            } else {
                // Middle elements: choose the maximum of the two possible parents [cite: 41]
                dp[i][j] = max(dp[i-1][j], dp[i-1][j-1]) + triangle[i][j];
            }
        }
    }

    // The answer is the maximum value in the last level [cite: 36]
    int max_sum = 0;
    for (int j = 0; j < N; j++) {
        max_sum = max(max_sum, dp[N-1][j]);
    }

    cout << max_sum << endl;

    return 0;
}
