#include <iostream>
#include <vector>

using namespace std;

typedef long long ll;

/**
 * Calculates the total number of customers seated by time 't'.
 * Each chef i serves a customer at time 0, T_i, 2*T_i, etc.
 * Total for chef i is floor(t / T_i) + 1.
 */
ll count_customers(ll t, int N, const vector<int>& T) {
    ll total = 0;
    for (int i = 0; i < N; ++i) {
        total += (t / T[i]) + 1;
        // Cap the total to prevent overflow beyond customer labels
        if (total > 2e12) return total; 
    }
    return total;
}

void solve() {
    int N, A;
    // Input N chefs and A customers to query [cite: 13]
    if (!(cin >> N >> A)) return;

    vector<int> T(N);
    for (int i = 0; i < N; ++i) {
        cin >> T[i]; // Input T_i for each chef [cite: 13]
    }

    for (int i = 0; i < A; ++i) {
        ll C;
        cin >> C; // Input the label of the customer [cite: 15, 16]

        // The first N customers are seated at time 0 
        if (C <= N) {
            cout << 0 << endl;
            continue;
        }

        // Binary search for the smallest time 't' where count_customers(t) >= C
        ll low = 0;
        ll high = 1e15; // Max time: C (10^12) * T_i (1000)
        ll result_t = high;

        while (low <= high) {
            ll mid = low + (high - low) / 2;
            if (count_customers(mid, N, T) >= C) {
                result_t = mid;
                high = mid - 1;
            } else {
                low = mid + 1;
            }
        }

        // Output the seating time [cite: 18]
        cout << result_t << endl;
    }
}

int main() {
    // Optimize I/O for large inputs
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    solve();
    return 0;
}
