#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
using namespace std;

bool check(const vector<int> &v, int start, int stop) {
    int length = stop - start + 1;

    // Base Case: size 2^1 must be [0, 1] 
    if (length == 2) {
        return (v[start] == 0 && v[stop] == 1);
    }

    int mid = start + (length / 2) - 1;

    // Split into left half (a) and right half (b) 
    // 1. b must be a valid virus
    bool b_is_valid = check(v, mid + 1, stop);
    if (!b_is_valid) return false;

    // 2. a must be a valid virus OR the reverse of a must be a valid virus 
    // Check normal a
    if (check(v, start, mid)) return true;

    // Check reversed a
    vector<int> a_rev;
    for (int i = mid; i >= start; i--) {
        a_rev.push_back(v[i]);
    }
    if (check(a_rev, 0, a_rev.size() - 1)) return true;

    return false;
}
int main(){
  int n,m;
  cin >> n >> m;
  for(int i = 0; i < n; i++){
    vector<int> v(std::pow(2,m));
    for(int i=0; i < std::pow(2,m);i++){
      cin >> v[i];
    }
    bool res = check(v,0,v.size()-1);
    if(res) cout <<"yes"<<endl;
    else cout << "no" << endl;
  }
}
