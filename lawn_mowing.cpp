#include <iostream>
#include <vector>

using namespace std;

int main() {
    // ปรับปรุงความเร็วในการรับส่งข้อมูลตามคำแนะนำของโจทย์
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, m;
    long long k;
    
    // รับค่า n, m, k
    if (!(cin >> n >> m >> k)) return 0;

    vector<long long> A(n);
    vector<long long> pref_grass(n + 1, 0);

    // เก็บค่า Prefix Sum เฉพาะปริมาณหญ้า
    for (int i = 0; i < n; ++i) {
        cin >> A[i];
        pref_grass[i + 1] = pref_grass[i] + A[i];
    }

    // ประมวลผลแต่ละคำถาม
    for (int i = 0; i < m; ++i) {
        int start;
        long long b;
        
        cin >> start >> b;

        // เริ่มต้น Binary Search เพื่อหา index ปลายทาง (r) ที่ไกลที่สุดที่งบประมาณจ่ายไหว
        long long low = start;
        long long high = n - 1;
        long long best_r = start - 1; // เก็บตำแหน่ง r ที่ดีที่สุด (เริ่มต้นที่ -1 แปลว่ายังตัดไม่ได้เลย)

        while (low <= high) {
            long long mid = low + (high - low) / 2;
            
            // ปริมาณหญ้าที่ตัดได้ตั้งแต่ start ถึง mid
            long long total_grass = pref_grass[mid + 1] - pref_grass[start];
            
            // จำนวนช่องที่ตัด
            long long num_cells = mid - start + 1;
            
            // คำนวณค่าใช้จ่ายทั้งหมด: ปริมาณหญ้า + (จำนวนช่อง * ค่าแรง k) ตามโจทย์เป๊ะๆ
            long long total_cost = total_grass + (num_cells * k);

            if (total_cost <= b) {
                // ถ้างบประมาณพอจ่าย ให้จำตำแหน่งนี้ไว้ แล้วลองขยับไปตัดช่องที่ไกลขึ้น
                best_r = mid;
                low = mid + 1;
            } else {
                // ถ้างบไม่พอ ต้องลดระยะทางลง
                high = mid - 1;
            }
        }

        // หากตำแหน่งที่ดีที่สุดน้อยกว่าจุดเริ่มต้น แปลว่างบไม่พอแม้แต่ช่องแรก
        if (best_r < start) {
            cout << 0 << "\n";
        } else {
            // แสดงผลปริมาณหญ้าที่มากที่สุดที่ตัดได้ [cite: 5, 11]
            cout << pref_grass[best_r + 1] - pref_grass[start] << "\n";
        }
    }

    return 0;
}
