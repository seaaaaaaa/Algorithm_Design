#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;


int bi(int n,int k){
  if(n==1||n==0) return 1;
  if(k==0||k==n) return 1;
  return bi(n-1,k-1) + bi(n-1,k);
}





int main(){
  int n,k;
  cin >> n >> k;
  cout << bi(n,k) << endl;
  return 0;
}
