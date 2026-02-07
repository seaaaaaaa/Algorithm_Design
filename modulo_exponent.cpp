#include <iostream>
#include <algorithm>
#include <cmath>
using namespace std;

int mod_expo(int a, int n , int k){
  if(n==1) return a%k;
  if(n%2==0){
    int tmp = mod_expo(a,n/2,k);
    return (tmp*tmp)%k;
  } else {
    int tmp = mod_expo(a,n/2,k);
    tmp = (tmp*tmp)%k;
    return (tmp*(a%k))%k;
  }
}

int main(){
  int x,n,k;
  cin >> x >> n >> k;
  cout << mod_expo(x,n,k) << endl;
  return 0;
}
