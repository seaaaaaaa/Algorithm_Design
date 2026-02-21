#include <iostream>
#include <algorithm>

using namespace std;

// ฟังก์ชันหาความยาวรวมของลำดับที่เกิดจากตัวเลข n
long long get_length(long long n) {
    long long len = 1;
    long long temp = n;
    while (temp > 1) {
        temp /= 2;
        len = len * 2 + 1;
    }
    return len;
}

// ฟังก์ชัน Divide and Conquer เพื่อนับเลข 1
long long count_ones(long long n, long long l, long long r, long long len) {
    // ถ้าระยะ l ถึง r ครอบคลุมทั้งก้อนนี้ จำนวนเลข 1 จะเท่ากับ n พอดี (คุณสมบัติพิเศษ)
    if (l <= 1 && r >= len) return n;
    
    // Base cases
    if (n == 0) return 0;
    if (n == 1) return 1;

    // หาตำแหน่งตรงกลางของก้อนปัจจุบัน (1-based index)
    long long mid = len / 2 + 1;
    long long ans = 0;

    // 1. ถ้าช่วง l ถึง r กินพื้นที่ฝั่งซ้าย
    if (l < mid) {
        ans += count_ones(n / 2, l, min(r, mid - 1), len / 2);
    }
    
    // 2. ถ้าช่วง l ถึง r คร่อมตำแหน่งตรงกลางพอดี
    if (l <= mid && mid <= r) {
        ans += n % 2;
    }
    
    // 3. ถ้าช่วง l ถึง r กินพื้นที่ฝั่งขวา
    if (r > mid) {
        // ต้อง shift ตำแหน่ง index ให้กลับไปเริ่มที่ 1 สำหรับการคำนวณในก้อนลูกขวา
        ans += count_ones(n / 2, max(1LL, l - mid), r - mid, len / 2);
    }

    return ans;
}

int main() {
    // ปรับความเร็ว I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    long long n, l, r;
    
    // รับข้อมูล 3 ตัว [cite: 41]
    if (cin >> n >> l >> r) {
        if (n == 0) {
            cout << 0 << "\n";
        } else {
            // หาความยาวสูงสุดของก้อน n ก่อน
            long long len = get_length(n);
            
            // คำนวณและแสดงผล
            cout << count_ones(n, l, r, len) << "\n";
        }
    }

    return 0;
}
