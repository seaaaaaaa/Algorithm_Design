#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
using namespace std;

int main(){
  int n;
  cin >> n;
  vector<set<int>> preq(n);
  vector<set<int>> adj(n);
  set<int> nsuc;
#include <iostream>
#include <vector>
#include <queue>

using namespace std;

int main() {
    int n;
    if (!(cin >> n)) return 0;

    // adj[x] stores all activities that depend on x being finished
    vector<vector<int>> adj(n); 
    
    // in_degree[i] stores the NUMBER of prerequisites activity i currently has
    vector<int> in_degree(n, 0); 

    // Read the M prerequisite activities [cite: 11]
    for (int i = 0; i < n; i++) {
        int m;
        cin >> m; // [cite: 10]
        for (int j = 0; j < m; j++) {
            int x;
            cin >> x;
            adj[x].push_back(i); // x must be completed before i
            in_degree[i]++;      // activity i has one more prerequisite
        }
    }

    queue<int> q;
    vector<int> res;

    // Step 1: Find all activities that can be started immediately (0 prerequisites)
    for (int i = 0; i < n; i++) {
        if (in_degree[i] == 0) {
            q.push(i);
        }
    }

    // Step 2: Process the queue
    while (!q.empty()) {
        int curr = q.front();
        q.pop();
        res.push_back(curr);

        // Tell all dependent activities that 'curr' is finished
        for (int dependent_activity : adj[curr]) {
            in_degree[dependent_activity]--; // One less prerequisite to wait for!
            
            // If it has no more prerequisites, it's ready to go
            if (in_degree[dependent_activity] == 0) {
                q.push(dependent_activity);
            }
        }
    }

    // Output the valid list of activities [cite: 13]
    for (int i = 0; i < res.size(); i++) {
        cout << res[i] << (i == res.size() - 1 ? "" : " ");
    }
    cout << "\n";

    return 0;
}
