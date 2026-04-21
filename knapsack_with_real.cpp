#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>

using namespace std;

struct Item {
    double v, w, ratio;
};

bool compareItems(Item a, Item b) {
    return a.ratio > b.ratio;
}

double maxp = 0;
double W;
int n;
vector<Item> items;

// This function calculates the tightest possible upper bound
double getBound(int step, double rw) {
    double bound = 0;
    for (int i = step; i < n; i++) {
        if (items[i].w <= rw) {
            rw -= items[i].w;
            bound += items[i].v;
        } else {
            // Take the fractional part of the next best item
            bound += items[i].v * (rw / items[i].w);
            break;
        }
    }
    return bound;
}

void knapsack(int step, double sumP, double sumW) {
    // 1. Update global max
    if (sumP > maxp) maxp = sumP;

    // 2. Base case: no more items
    if (step == n) return;

    // 3. PRUNING: Use the fractional bound
    // If current points + best possible fraction < current best, STOP.
    if (sumP + getBound(step, W - sumW) <= maxp + 1e-9) return;

    // 4. Branching Strategy: Try "Take" then "Leave"
    // Since we sorted by density, "Take" usually finds the max faster
    if (sumW + items[step].w <= W) {
        knapsack(step + 1, sumP + items[step].v, sumW + items[step].w);
    }
    
    knapsack(step + 1, sumP, sumW);
}

int main() {
    ios_base::sync_with_stdio(false); // Fast I/O
    cin.tie(NULL);

    if (!(cin >> W >> n)) return 0;

    vector<double> v_t(n), w_t(n);
    for (int i = 0; i < n; i++) cin >> v_t[i];
    for (int i = 0; i < n; i++) cin >> w_t[i];

    for (int i = 0; i < n; i++) {
        items.push_back({v_t[i], w_t[i], v_t[i] / w_t[i]});
    }

    // Sort items by density
    sort(items.begin(), items.end(), compareItems);

    knapsack(0, 0, 0);

    cout << fixed << setprecision(4) << maxp << endl;

    return 0;
}
