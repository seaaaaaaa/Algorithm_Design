#include <iostream>
#include <vector>

using namespace std;

int getsum(const vector<int> &S, int a, int b){
    return S[b] - S[a-1];
}

int mss(const vector<int> &A,int start, int stop, const vector<int> &S){
    if(start == stop){
        return A[start];
    }
    int m = (start + stop)/2;
    int r1 = mss(A,start,m,S);
    int r2 = mss(A,m+1,stop,S);

    int max_sum_left = getsum(S,m,m);
    for(int i=start;i<m;i++){
        max_sum_left = max(max_sum_left,getsum(S,i,m));
    }

    int max_sum_right = getsum(S,m+1,m+1);
    for(int i=m+2;i<=stop;i++){
        max_sum_right = max(max_sum_right,getsum(S,m+1,i));
    }

    int r3 = max_sum_left + max_sum_right;
    int m1 = max(r1,r2);
    return max(m1,r3);
}

int main(){
    ios_base::sync_with_stdio(false);cin.tie(0);
    int n;
    cin >> n;
    vector<int> v(n);
    vector<int> nv(n);
    vector<int> s(n);
    vector<int> ns(n);
    for(int i=0;i<n;i++){
        cin >> v[i];
        nv[i] = v[i] * -1;
        if(i==0){
            s[i] = v[i];
        } else{
            s[i] = s[i-1] + v[i];
        }
        ns[i] = s[i] * -1;
    }
    // for(auto it : v){
    //     cout << it << " ";
    // }
    // cout << "\n";
    // for(auto it : s){
    //     cout << it << " ";
    // }
    int ans = mss(v,0,n-1,s);
    int min_sub = mss(nv,0,n-1,ns);
    // cout << min_sub << " " << s[n-1] << "\n";
    // cout << s[n-1] + min_sub << "\n";
    cout << max(ans,s[n-1] + min_sub) << "\n";
}
