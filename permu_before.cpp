#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
using namespace std;

void perm(int n,vector<int> &sol,int len,vector<bool> &used,map<int,int> &p) {
    if (len < n) {
        for (int i = 0;i<n;i++) {
            if (used[i] == false) {
                if(p.find(i)==p.end()||(p.find(i) != p.end() && used[p[i]]==true)){
                    used[i] = true;
                    sol[len] = i;
                    perm(n,sol,len+1,used,p);
                    used[i] = false;
                }    
            }
        }
    }else {
        for (auto &x : sol) cout << x << " ";
        cout << endl;
    }
}

int main() {
    int n,m;
    cin >> n >> m;
    map<int,int> p;
    for(int i = 0;i<m;i++) {
        int a,b;
        cin >> a >> b;
        p[b] = a;
    }
    vector<int> sol(n);
    vector<bool> used(n,false);
    perm(n,sol,0,used,p);
    return 0;
}
