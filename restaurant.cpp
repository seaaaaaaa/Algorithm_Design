#include <iostream>
#include <vector> 
#include <queue>
using namespace std;
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    int N,M;
    cin >> N >> M ;
    vector<int> T(N+1);
    for (int i=1; i<=N; i++){
        cin>>T[i];
    }
    priority_queue<pair<long long,int>,vector<pair<long long,int>>,greater<pair<long long,int>>> pq;
    for (int i=1;i<=N;i++){
        pq.push({0,i});
    }
    for (int customer =0;  customer < M; customer++){
        int x;
        cin >> x;
        while(pq.top().first!=x) {
            auto [availableTime, chefId] = pq.top();
            long long newAvailableTime=availableTime+T[chefId];
            pq.push({newAvailableTime,chefId});
            pq.pop();
        }
        auto [availableTime, chefId] = pq.top();
        pq.pop();
        cout<< availableTime<<endl;
        long long newAvailableTime=availableTime+T[chefId];
        pq.push({newAvailableTime,chefId});
    }
    return 0;

} 
