#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    // ปรับแต่งความเร็ว I/O ของ C++
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N, M;
    if (!(cin >> N >> M)) return 0;

    vector<int> coins(N);
    for (int i = 0; i < N; ++i) {
        cin >> coins[i];
    }

    // สร้างตาราง DP ขนาด M + 1 
    // กำหนดค่าเริ่มต้นเป็นค่าที่สูงกว่าความเป็นไปได้มากที่สุด (เช่น M + 1)
    // เพื่อเอาไว้ใช้เปรียบเทียบหาค่า min()
    vector<int> dp(M + 1, M + 1);
    
    // Base case: ทอนเงิน 0 บาท ใช้เหรียญ 0 เหรียญ
    dp[0] = 0;

    // คำนวณ DP Bottom-Up ตั้งแต่เงิน 1 บาท ไปจนถึง M บาท
    for (int i = 1; i <= M; ++i) {
        // ลองใช้เหรียญทุกชนิดที่มี
        for (int j = 0; j < N; ++j) {
            int c = coins[j];
            // ถ้ามูลค่าเหรียญ c ไม่เกินจำนวนเงิน i ที่กำลังหา
            if (i >= c) {
                dp[i] = min(dp[i], dp[i - c] + 1);
            }
        }
    }

    // พิมพ์คำตอบของจำนวนเงิน M บาท
    cout << dp[M] << "\n";

    return 0;
}
