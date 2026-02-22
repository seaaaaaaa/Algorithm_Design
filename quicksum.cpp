#include <iostream>
#include <vector>

using namespace std;

int main() {
    // Optimize standard I/O operations for performance
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N, M, K;
    // Read N (rows), M (columns), and K (queries)
    if (!(cin >> N >> M >> K)) return 0;

    // Create a 2D prefix sum array P initialized to 0. 
    // We use (N+1) x (M+1) for 1-based indexing to handle boundaries easily.
    vector<vector<long long>> P(N + 1, vector<long long>(M + 1, 0));

    // Read the array A and build the prefix sum array P on the fly
    for (int i = 1; i <= N; ++i) {
        for (int j = 1; j <= M; ++j) {
            long long val;
            cin >> val;
            
            // Formula for building the 2D prefix sum
            P[i][j] = P[i - 1][j] + P[i][j - 1] - P[i - 1][j - 1] + val;
        }
    }

    // Process the K queries
    for (int k = 0; k < K; ++k) {
        int r1, c1, r2, c2;
        cin >> r1 >> c1 >> r2 >> c2;
        
        // Convert the 0-based input indices to 1-based indices
        r1++; c1++; r2++; c2++;

        // Calculate the block sum in O(1) time
        long long block_sum = P[r2][c2] - P[r1 - 1][c2] - P[r2][c1 - 1] + P[r1 - 1][c1 - 1];
        
        // Print the result for this query
        cout << block_sum << "\n";
    }

    return 0;
}
