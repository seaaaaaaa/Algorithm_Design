#include <iostream>
#include <vector>
#include <set>

using namespace std;
typedef long long ll;

int main(){
    ios_base::sync_with_stdio(false);cin.tie(0);
    ll n,k;
    cin >> n >> k;
    vector<ll> a(k+1,0);
    set<ll> apos;
    for(ll i=1;i<=k;i++){
        ll banana;
        cin >> banana;
        // cin >> a[i];
        apos.insert(banana);
    }
    vector<vector<ll>> dp(n+1,vector<ll>(3,0));
    ll mod = 1000000007;
    // ll idx = 0;
    // ll cur_a = a[idx];
    for(ll i=0;i<=n;i++){
        if(i == 0){
            dp[i][0] = 1;
            dp[i][1] = 0;
            dp[i][2] = 0;
        } else {
            dp[i][0] = ((dp[i-1][0] % mod) + (dp[i-1][1])% mod) % mod;
            if(i >= 2){
                if(apos.find(i-1) == apos.end()){
                    dp[i][1] = ((dp[i-2][0] % mod) + (dp[i-2][2] % mod)) % mod;
                }
            }
            if(i >= 4){
                if(!(apos.find(i-1) != apos.end() || apos.find(i-2) != apos.end() || apos.find(i-3) != apos.end() )){    
                    if(i == 4){
                        dp[i][2] = 1;
                    } else {
                        dp[i][2] = ((dp[i-4][2]% mod) + (dp[i-4][1]% mod)) % mod;
                    }
                }
            }
        }
    }
    cout << ((dp[n][0]% mod) + (dp[n][1]% mod) + (dp[n][2]% mod)) % mod << "\n";
    // for(ll i=0;i<=n;i++){
    //     for(ll j=0;j<5;j++){
    //         cout << dp[i][j] << " ";
    //     }
    //     cout << "\n";
    // }
    return 0;
}
