#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    // เพิ่มความเร็วในการรับส่งข้อมูลตามคำแนะนำของโจทย์
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    if (!(cin >> n)) return 0;

    vector<int> A(n);
    for (int i = 0; i < n; ++i) {
        cin >> A[i];
    }

    // กรณีที่อาร์เรย์มีขนาดเพียง 1 ช่อง จะเลือกได้แค่ช่วงเดียว
    if (n == 1) {
        cout << A[0] << "\n";
        return 0;
    }

    // L[i] เก็บค่า MCS จาก A[0] ถึง A[i]
    vector<long long> L(n);
    long long current_max_L = A[0];
    L[0] = A[0];
    for (int i = 1; i < n; ++i) {
        current_max_L = max((long long)A[i], current_max_L + A[i]);
        L[i] = max(L[i - 1], current_max_L);
    }

    // R[i] เก็บค่า MCS จาก A[i] ถึง A[n-1]
    vector<long long> R(n);
    long long current_max_R = A[n - 1];
    R[n - 1] = A[n - 1];
    for (int i = n - 2; i >= 0; --i) {
        current_max_R = max((long long)A[i], current_max_R + A[i]);
        R[i] = max(R[i + 1], current_max_R);
    }

    // ค่าเริ่มต้นคือกรณีที่เลือกช่วงติดกันเพียงช่วงเดียว (หาค่า MCS ของทั้งอาร์เรย์)
    long long max_sum = L[n - 1]; 
    
    // ตรวจสอบกรณีที่แบ่งเป็น 2 ช่วงที่ไม่ซ้อนทับกัน (จุดแบ่งคือ i และ i+1)
    for (int i = 0; i < n - 1; ++i) {
        max_sum = max(max_sum, L[i] + R[i + 1]);
    }

    cout << max_sum << "\n";

    return 0;
}
