#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

int main() {
    // Speed up input and output
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    if (!(cin >> n)) return 0; // [cite: 46]

    vector<long long> a(n + 1);
    for (int i = 1; i <= n; i++) {
        cin >> a[i]; // [cite: 48, 49]
    }

    // map to store the minimum prefix sum S[p-1] encountered before value x
    // Key: the value A[i], Value: the minimum S[i-1] seen so far for that key
    map<long long, long long> min_prefix;
    
    long long current_prefix_sum = 0;
    long long max_double_end_sum = -2e18; // Initialize with a very small number

    for (int i = 1; i <= n; i++) {
        long long val = a[i];
        long long s_before = current_prefix_sum; // This is S[i-1]
        current_prefix_sum += val; // This is now S[i]

        // If we've seen this value before, it can form a Double End Sequence 
        if (min_prefix.count(val)) {
            // Sum = S[i] - min(S[p-1]) where A[p] == val
            long long current_sum = current_prefix_sum - min_prefix[val];
            max_double_end_sum = max(max_double_end_sum, current_sum);
            
            // Also update the minimum prefix for this value if S[i-1] is smaller
            min_prefix[val] = min(min_prefix[val], s_before);
        } else {
            // First time seeing this value, it's a sequence of length 1 
            max_double_end_sum = max(max_double_end_sum, val);
            min_prefix[val] = s_before;
        }
    }

    cout << max_double_end_sum << endl; // [cite: 51]

    return 0;
 }
