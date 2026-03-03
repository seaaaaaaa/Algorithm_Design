#include <iostream>
#include <vector>

using namespace std;

int main() {
    // ปรับแต่งความเร็ว I/O ของ C++
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, k;
    // รับค่า n (จำนวนชิ้นช็อกโกแลต) และ k (ขนาดของเซ็ต S)
    if (!(cin >> n >> k)) return 0;

    vector<int> S(k);
    for (int i = 0; i < k; ++i) {
        cin >> S[i];
    }

    int MOD = 1000003;
    
    // สร้างตาราง DP ขนาด n + 1 และกำหนดค่าเริ่มต้นเป็น 0
    vector<long long> dp(n + 1, 0);
    
    // Base case: จำนวนวิธีกินช็อกโกแลต 0 ชิ้น คือ 1 วิธี
    dp[0] = 1;

    // คำนวณ DP แบบ Bottom-Up
    for (int i = 1; i <= n; ++i) {
        // ลองหักช็อกโกแลตตามจำนวนชิ้นที่อนุญาตในเซ็ต S
        for (int j = 0; j < k; ++j) {
            int s = S[j];
            // ถ้าจำนวนชิ้นช็อกโกแลตปัจจุบัน (i) มากกว่าหรือเท่ากับขนาดที่หักกินได้ (s)
            if (i >= s) {
                dp[i] = (dp[i] + dp[i - s]) % MOD;
            }
        }
    }

    // พิมพ์ผลลัพธ์จำนวนวิธีทั้งหมดในการกินช็อกโกแลต n ชิ้น
    cout << dp[n] << "\n";

    return 0;
}
