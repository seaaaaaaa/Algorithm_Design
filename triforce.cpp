#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int type(vector<vector<int>> &v,int vs,int ve,int hs,int he){
  if(vs+1==ve&&hs+1==he){
    if(v[ve][he]!=0)return 0;
    if(v[vs][hs]!=0&&v[vs][he]!=0&&v[ve][hs]!=0){
      if(v[vs][hs]==v[vs][he]) return v[vs][hs];
      if(v[vs][he]==v[ve][hs]) return v[vs][he];
      if(v[ve][hs]==v[vs][hs]) return v[ve][hs];
    }
    return 0;
  }
  int m = (ve-vs)/2;
  bool check4 = true;
  for(int i=vs+m+1;i<=ve;i++){
    for(int j=hs+m+1;j<=he;j++)
      if(v[i][j]!=0)check4=false;
  }
  if(!check4) return 0;
  int type1 = type(v,vs,vs+m,hs,hs+m);
  int type2 = type(v,vs,vs+m,hs+m+1,he);
  int type3 = type(v,vs+m+1,ve,hs,hs+m);
  if(type1!=0&&type2!=0&&type3!=0){
    if(type1==type2) return type1;
    if(type2==type3) return type2;
    if(type1==type3) return type3;
  }
  return 0;

  

}

int main(){
  for(int i=0;i<3;i++){
    int n;
    cin >> n;
    vector<vector<int>> v(n, vector<int>(n));
    for(int j = 0; j < n; j++){
      for(int k = 0; k < n; k++){
        int x;
        cin >> v[j][k];
      }
    }
    cout << type(v,0,v.size()-1,0,v.size()-1)<< endl;
  }
}
