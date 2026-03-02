#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
int main(){
  int N,K;
  cin >> N >> K;
  vector<int> n(N);
  vector<int> k(K);
  for(int i=0;i<N;i++)cin>>n[i];
  for(int i=0;i<K;i++)cin>>k[i];
  vector<int> dp(N);
  dp[0] = n[0];
  for(int i=1;i<N;i++){
    dp[i] = -100000000;
    for(int j = 1;j<=min(i,K);j++)dp[i]=max(dp[i-j]+n[i]-k[j-1],dp[i]);
  }
  cout << dp[N-1] << endl;
  
}
