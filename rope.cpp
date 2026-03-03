#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    int n, a, b, c;
    if (!(cin >> n >> a >> b >> c)) return 0;

    // dp[i] stores the max pieces for a ribbon of length i
    // Initialize with a very small number to represent "impossible"
    vector<int> dp(n + 1, -1e9); 
    
    // Base case: a ribbon of length 0 has 0 pieces
    dp[0] = 0;

    vector<int> sizes = {a, b, c};

    for (int i = 1; i <= n; i++) {
        for (int s : sizes) {
            if (i >= s) {
                // If we take a piece of size 's', the total pieces is 
                // 1 + whatever the max pieces were for the remaining length (i-s)
                dp[i] = max(dp[i], dp[i - s] + 1);
            }
        }
    }

    cout << dp[n] << endl;
    return 0;
}
