#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

void fillMatrix(vector<vector<int>> &v, int a, int b, int r_start, int c_start) {
    // Base Case
    if (a == 0) {
        v[r_start][c_start] = b; 
        return;
    }

    // Calculate the size of the smaller sub-matrix
    int half = 1 << (a - 1);

    // Top-Left: H(a-1, b)
    fillMatrix(v, a - 1, b, r_start, c_start);
    
    // Top-Right: H(a-1, b-1)
    fillMatrix(v, a - 1, b - 1, r_start, c_start + half); 
    
    // Bottom-Left: H(a-1, b+1)
    fillMatrix(v, a - 1, b + 1, r_start + half, c_start); 
    
    // Bottom-Right: H(a-1, b)
    fillMatrix(v, a - 1, b, r_start + half, c_start + half); 
}

int main(){
  int a,b;
  cin >> a >> b;
  vector<vector<int>> v(1<<a,vector<int>(1<<a)) ;
  fillMatrix(v,a,b,0,0);
  for(int i=0 ; i<1<<a; i++){
    for(int j=0; j<1<<a; j++){
      cout << v[i][j] << " ";
    }
    cout << endl;
  }
  return 0;
}

