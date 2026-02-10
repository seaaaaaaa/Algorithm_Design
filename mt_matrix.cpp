#include <iostream>

using namespace std;

// Global variables for the base 2x2 matrix values
long long u, v, w, p;

/**
 * Recursive function to find the value at row r, column c in matrix M_n.
 * n: The order of the matrix.
 * r: The row index (1-based).
 * c: The column index (1-based).
 * sign: Tracks the current sign (1 or -1) due to the bottom quadrants.
 */
long long findValue(int n, long long r, long long c, int sign) {
    // Base case: When n is 1, return the value from the defined 2x2 matrix [cite: 5, 8]
    if (n == 1) {
        if (r == 1 && c == 1) return sign * u;
        if (r == 1 && c == 2) return sign * v;
        if (r == 2 && c == 1) return sign * w;
        if (r == 2 && c == 2) return sign * p;
    }

    // Midpoint: 2^(n-1). Used to determine which block the coordinate belongs to[cite: 6, 35].
    long long L = 1LL << (n - 1);

    // M_n = [  M_n-1     (M_n-1)^T  ] 
    //       [ -M_n-1    (-M_n-1)^T  ]

    if (r <= L && c <= L) {
        // Top-Left: Standard M_n-1
        return findValue(n - 1, r, c, sign);
    } 
    else if (r <= L && c > L) {
        // Top-Right: Transpose of M_n-1 -> swap row and column 
        return findValue(n - 1, c - L, r, sign);
    } 
    else if (r > L && c <= L) {
        // Bottom-Left: Negative of M_n-1 
        return findValue(n - 1, r - L, c, -sign);
    } 
    else {
        // Bottom-Right: Negative Transpose of M_n-1 
        return findValue(n - 1, c - L, r - L, -sign);
    }
}

int main() {
    // Speed up I/O for large number of queries [cite: 17]
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    int m;
    
    // Read n (order) and m (number of queries) [cite: 17]
    if (!(cin >> n >> m)) return 0;
    
    // Read u, v, w, p [cite: 17]
    cin >> u >> v >> w >> p;

    // Process m queries [cite: 18, 20]
    while (m--) {
        long long r, c;
        cin >> r >> c;
        cout << findValue(n, r, c, 1) << "\n";
    }

    return 0;
}
