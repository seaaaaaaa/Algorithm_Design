#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    // Fast I/O
    std::ios_base::sync_with_stdio(false); 
    std::cin.tie(0);

    int n, q;
    if (!(cin >> n >> q)) return 0;

    // v stores pairs of {value, count}
    vector<pair<long long, long long>> v;
    for(int i = 0; i < n; i++){
        long long x, y;
        cin >> x >> y;
        v.push_back(make_pair(x, y));
    }

    // Sort by value (x)
    sort(v.begin(), v.end());

    // Precompute the ending index (cumulative sum) for each distinct value
    // We use a separate vector for the counts to make binary search easy
    vector<long long> endpoints;
    long long current_count = 0;
    
    for(auto &p : v) {
        current_count += p.second; // Add the count (y)
        endpoints.push_back(current_count);
    }

    // Process queries
    for(int i = 0; i < q; i++){
        long long index;
        cin >> index;

        // lower_bound returns an iterator to the first element >= index
        // This effectively finds which block the index falls into.
        auto it = lower_bound(endpoints.begin(), endpoints.end(), index);
        
        // Calculate the position in the 'v' vector
        int pos = distance(endpoints.begin(), it);

        // Print the value associated with that block
        cout << v[pos].first << "\n"; // Use \n instead of endl for speed
    }

    return 0;
}



