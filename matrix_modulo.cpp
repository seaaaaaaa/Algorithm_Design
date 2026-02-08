#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
using namespace std;

vector<int> mat_mul(const vector<int> &m, const vector<int> &n, int k){
  vector<int> res(4);
  res[0] = ((((m[0]%k)*(n[0]%k))%k)+(((m[1]%k)*(n[2]%k))%k))%k;
  res[1] = ((((m[0]%k)*(n[1]%k))%k)+(((m[1]%k)*(n[3]%k))%k))%k;
  res[2] = ((((m[2]%k)*(n[0]%k))%k)+(((m[3]%k)*(n[2]%k))%k))%k;
  res[3] = ((((m[2]%k)*(n[1]%k))%k)+(((m[3]%k)*(n[3]%k))%k))%k;
  return res;
}

vector<int> power(vector<int> A, int n, int k) {
    // Initialize result as Identity Matrix [1 0; 0 1]
    vector<int> res = {1, 0, 0, 1};
    
    while (n > 0) {
        // If n is odd, multiply the current result by A
        if (n % 2 == 1) {
            res = mat_mul(res, A, k);

        }
        // Square A and divide n by 2
        A = mat_mul(A, A, k);
        n /= 2;
    }
    return res;
}

int main(){
  int n,k;
  cin >> n >> k;
  vector<int> v(4);
  
  for(int i = 0; i<4; i++){
    cin >> v[i];
  }
  vector<int> res = power(v,n,k);
  for(auto &x: res) cout << x << " ";
  return 0;
}
