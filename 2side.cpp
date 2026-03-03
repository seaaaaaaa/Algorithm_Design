#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    // ปรับแต่งความเร็ว I/O ของ C++ ตามคำแนะนำโจทย์ [cite: 78]
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, w, k;
    if (!(cin >> n >> w >> k)) return 0;

    vector<long long> l(n + 1);
    for (int i = 1; i <= n; ++i) {
        cin >> l[i];
    }

    vector<long long> r(n + 1);
    for (int i = 1; i <= n; ++i) {
        cin >> r[i];
    }

    // จำนวนป้ายมากที่สุดที่เป็นไปได้จากขีดจำกัดระยะห่าง w
    int max_possible_billboards = (n - 1) / (w + 1) + 1;

    // แยก Case: ถ้า k มากพอจนไม่มีผลกระทบ (หรือ k == n) จะใช้ O(N)
    if (k >= max_possible_billboards || k == n) {
        vector<long long> max_L(n + 1, 0);
        vector<long long> max_R(n + 1, 0);

        for (int i = 1; i <= n; ++i) {
            int prev_j = max(0, i - w - 1);
            
            // คำนวณแบบสลับฝั่ง ซ้ายสลับขวา และ ขวาสลับซ้าย 
            long long dp_L = l[i] + max_R[prev_j];
            long long dp_R = r[i] + max_L[prev_j];

            max_L[i] = max(max_L[i - 1], dp_L);
            max_R[i] = max(max_R[i - 1], dp_R);
        }

        cout << max(max_L[n], max_R[n]) << "\n";
    } 
    // แยก Case: ถ้า k เข้ามามีบทบาท (กรณีของคะแนนโบนัส k=50 ) จะใช้ O(N * K)
    else {
        vector<long long> prev_max_L(n + 1, -1);
        vector<long long> prev_max_R(n + 1, -1);
        long long ans = 0;

        for (int c = 1; c <= k; ++c) {
            vector<long long> curr_max_L(n + 1, -1);
            vector<long long> curr_max_R(n + 1, -1);

            for (int i = 1; i <= n; ++i) {
                int prev_j = max(0, i - w - 1);
                long long dp_L = -1, dp_R = -1;

                if (c == 1) {
                    dp_L = l[i];
                    dp_R = r[i];
                } else {
                    // ตรวจสอบเพื่อดึงค่าสูงสุดในอดีต (ที่ตำแหน่งปลอดภัยจากกฎ a-w ถึง a+w )
                    if (prev_j >= 1) {
                        if (prev_max_R[prev_j] != -1) dp_L = l[i] + prev_max_R[prev_j];
                        if (prev_max_L[prev_j] != -1) dp_R = r[i] + prev_max_L[prev_j];
                    }
                }

                // เก็บ Rolling Max เพื่อการนำไปใช้ของป้ายอันต่อไป
                curr_max_L[i] = curr_max_L[i - 1];
                if (dp_L != -1) curr_max_L[i] = max(curr_max_L[i], dp_L);

                curr_max_R[i] = curr_max_R[i - 1];
                if (dp_R != -1) curr_max_R[i] = max(curr_max_R[i], dp_R);

                // ตรวจสอบค่าที่ดีที่สุดตลอดการคำนวณ
                if (curr_max_L[i] != -1) ans = max(ans, curr_max_L[i]);
                if (curr_max_R[i] != -1) ans = max(ans, curr_max_R[i]);
            }
            prev_max_L = curr_max_L;
            prev_max_R = curr_max_R;
        }
        cout << ans << "\n";
    }

    return 0;
}
