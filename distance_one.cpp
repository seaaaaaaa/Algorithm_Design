#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main (){
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  int n,k;
  cin >> n >> k;
  int mod = 100000007;
  vector<int> dp(n+1,0);
  dp[0]=1;
  for(int i = 1; i<=n; i++){
    dp[i]=dp[i-1];
    if( i>=k){
      dp[i] = (dp[i]+dp[i-k])%mod;
    } else {
      dp[i] = (dp[i]+1)%mod;
    }
  }

  cout << dp[n] << endl;
}



