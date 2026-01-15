#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

template <typename T>
void insertion_sort(vector<T> &A) {
    for (int pos = A.size()-2;pos >= 0;pos--) {
        T tmp = A[pos];
        size_t i = pos+1;
        while (i < A.size() && A[i] < tmp) {
            A[i-1] = A[i];
            i++;
        }
        A[i-1] = tmp;
    }
}

int main() {
    int n,m;
    cin >> n >> m;
    vector<int> A(n);
    for (int i = 0;i < n;i++) {
        cin >> A[i];
    }
    
    for (int i = 0;i < m;i++) {
        int k;
        cin >> k;
        int res=0;
        auto it = find(A.begin(), A.end(), k);
        for(auto i = A.begin(); i != it; i++) {
            if(*i > k) {
                res++;
            }
        }
        cout << res << endl;
    }
}