#include <iostream>
#include <vector>
using namespace std;

void perm_kn_replace(int n,vector<int> &sol,int len,int k) {
    if (len < k) {
        for (int i = 1;i<=n;i++) {
            sol[len] = i;
            perm_kn_replace(n,sol,len+1,k);
        }
    } else {
        for (auto &x : sol) cout << x;
        cout << endl;
    }
}

int main(){
    int n,k;
    cin >> n >> k;
    vector<int> sol(k);
    perm_kn_replace(n,sol,0,k);
    return 0;
}