#include <iostream>
#include <vector>
using namespace std;

void perm_kn_replace(int n,vector<int> &sol,int len,int k,int one) {
  
  //for(auto &x : sol) cout << x;
  //cout << endl;
  //cout << n << " " << len << " " << k<< " " << one << endl;
  if (len < k) {
      for (int i = 0;i<n;i++) {
        sol[len] = i;
        perm_kn_replace(2,sol,len+1,k,one);
      }
  } else {
    //cout << endl << " " << len << " " << k << endl;

    //for(auto &x : sol) cout << x;
    //cout << endl;
      
    int count = 0;
    for(auto &x :sol){
      if(x==1) {
        count++;
        if(count==one) break;
      }
      else count = 0;
    }
    if(count == one){
      for (auto &x : sol) cout << x;
      cout << endl;
        //cout << endl;
    }  
  }
}

int main(){
    int n,k;
    cin >> n >> k;
    vector<int> sol(n);
    perm_kn_replace(2,sol,0,n,k);
    return 0;
}
