#include <iostream>
#include <vector>

using namespace std;

int cc_num = 0;

void dfs_cc(int a, const vector<vector<int>>& adj, vector<int>& visited) {
    visited[a] = cc_num;
    for (int i = 0; i < adj.size(); i++) {
        if (adj[a][i] == 1 && visited[i] == 0) {
            dfs_cc(i, adj, visited);
        }
    }
}

int main() {
    int v, e;
    if (!(cin >> v >> e)) return 0;

    vector<vector<int>> adj(v, vector<int>(v, 0));
    for (int i = 0; i < e; i++) {
        int a, b;
        cin >> a >> b;
        // ADJUST HERE: subtract 1 from input to handle 1-indexing
        adj[a - 1][b - 1] = 1;
        adj[b - 1][a - 1] = 1;
    }

    vector<int> visited(v, 0);
    for (int i = 0; i < v; i++) {
        if (visited[i] == 0) {
            cc_num += 1;
            dfs_cc(i, adj, visited);
        }
    }
    cout << cc_num << endl;
    return 0;
}
