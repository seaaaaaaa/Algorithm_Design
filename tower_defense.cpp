#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n, m, k, w;
    cin >> n >> m >> k >> w;
    
    vector<int> p(m), h(m), t(k);
    
    // Read monster positions
    for (int i = 0; i < m; i++) {
        cin >> p[i];
    }
    
    // Read monster health
    for (int i = 0; i < m; i++) {
        cin >> h[i];
    }
    
    // Read tower positions
    for (int i = 0; i < k; i++) {
        cin >> t[i];
    }
    
    // Track current health of each monster
    vector<int> current_health = h;
    
    // For each tower, find and shoot the monster with highest health it can reach
    for (int i = 0; i < k; i++) {
        int tower_pos = t[i];
        int best_monster = -1;
        int best_health = 0;
        
        // Find the monster with highest health that this tower can reach
        for (int j = 0; j < m; j++) {
            if (current_health[j] > 0 && 
                abs(p[j] - tower_pos) <= w &&
                current_health[j] > best_health) {
                best_health = current_health[j];
                best_monster = j;
            }
        }
        
        // Shoot the best monster if found
        if (best_monster != -1) {
            current_health[best_monster]--;
        }
    }
    
    // Calculate total remaining health
    long long total = 0;
    for (int i = 0; i < m; i++) {
        total += current_health[i];
    }
    
    cout << total << endl;
    
    return 0;
}
