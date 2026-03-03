#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

int main() {
    // ปรับแต่งความเร็ว I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N, K;
    if (!(cin >> N >> K)) return 0;

    vector<int> C(N);
    for (int i = 0; i < N; ++i) {
        cin >> C[i];
    }

    vector<int> P(K);
    for (int i = 0; i < K; ++i) {
        cin >> P[i];
    }

    // ขั้นตอนที่ 1: สร้าง DFA (Deterministic Finite Automaton) สำหรับหา State ถัดไปของ Pattern
    // next_state[j][bit] เก็บค่า state ใหม่เมื่อเราอยู่ที่ state 'j' แล้วต่อท้ายด้วยค่า 'bit' (0 หรือ 1)
    vector<vector<int>> next_state(K, vector<int>(2, 0));
    for (int i = 0; i < K; ++i) {
        for (int bit = 0; bit <= 1; ++bit) {
            string s = "";
            for (int j = 0; j < i; ++j) s += to_string(P[j]);
            s += to_string(bit);

            int match_len = 0;
            // หา Prefix ของ Pattern ที่ยาวที่สุดที่ไปตรงกับ Suffix ของสตริง s
            for (int len = s.length(); len >= 1; --len) {
                bool ok = true;
                for (int k = 0; k < len; ++k) {
                    if (s[s.length() - len + k] - '0' != P[k]) {
                        ok = false;
                        break;
                    }
                }
                if (ok) {
                    match_len = len;
                    break;
                }
            }
            next_state[i][bit] = match_len;
        }
    }

    // ขั้นตอนที่ 2: สร้างและเริ่มต้นตาราง DP
    // dp[i][j][last_bit] = ผลรวมมากสุด (-1 หมายถึงรูปแบบที่เป็นไปไม่ได้)
    vector<vector<vector<long long>>> dp(N + 1, vector<vector<long long>>(K, vector<long long>(2, -1)));
    
    // Base case: เริ่มต้นที่ กม. 0, ระยะความตรง pattern เป็น 0, บิตจำลองก่อนหน้าเป็น 0 (เพราะสามารถเลือก 1 ต่อได้)
    dp[0][0][0] = 0;

    // ขั้นตอนที่ 3: คำนวณ DP Transitions
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < K; ++j) {
            for (int last = 0; last <= 1; ++last) {
                if (dp[i][j][last] == -1) continue;

                // กรณีที่ 1: เลือก "ไม่ติดป้าย" (เพิ่ม 0)
                int nj0 = next_state[j][0];
                if (nj0 < K) { // รอดจากกฎสมชาย (Pattern ต้องห้าม)
                    dp[i + 1][nj0][0] = max(dp[i + 1][nj0][0], dp[i][j][last]);
                }

                // กรณีที่ 2: เลือก "ติดป้าย" (เพิ่ม 1)
                if (last == 0) { // รอดจากกฎห้ามป้ายติดกัน (ต้องไม่เป็น 1 มาก่อน)
                    int nj1 = next_state[j][1];
                    if (nj1 < K) { // รอดจากกฎสมชาย (Pattern ต้องห้าม)
                        dp[i + 1][nj1][1] = max(dp[i + 1][nj1][1], dp[i][j][last] + C[i]);
                    }
                }
            }
        }
    }

    // ขั้นตอนที่ 4: หาผลรวมที่มากที่สุดของ DP ที่คำนวณเสร็จใน กม. ที่ N
    long long ans = 0;
    for (int j = 0; j < K; ++j) {
        for (int last = 0; last <= 1; ++last) {
            ans = max(ans, dp[N][j][last]);
        }
    }

    cout << ans << "\n";

    return 0;
}
