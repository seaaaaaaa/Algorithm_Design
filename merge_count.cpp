#include <iostream>
#include <vector>
#include <numeric>

using namespace std;

int n, k;
int current_calls = 1;
vector<int> arr;

void generate(int l, int r) {
    // If we reached k or cannot split further, return
    if (current_calls >= k || l + 1 == r) {
        return;
    }

    int mid = (l + r) / 2;

    // If we have budget for 2 more calls (left child + right child)
    if (current_calls + 2 <= k) {
        // Swap to break sortedness
        swap(arr[mid - 1], arr[mid]);
        
        current_calls += 2; // Add cost of children
        
        generate(l, mid);
        generate(mid, r);
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    if (!(cin >> n >> k)) return 0;

    if (k % 2 == 0 || k > 2 * n - 1) {
        cout << "-1" << endl;
        return 0;
    }

    // Initialize sorted array 1 to n
    arr.resize(n);
    iota(arr.begin(), arr.end(), 1);

    generate(0, n);

    if (current_calls == k) {
        for (int i = 0; i < n; i++) {
            cout << arr[i] << (i == n - 1 ? "" : " ");
        }
        cout << endl;
    } else {
        cout << "-1" << endl;
    }

    return 0;
}
