# Pitfalls — Complexity Analysis

1. Big-O ไม่ใช่ exact runtime
2. O ไม่ได้แปลว่า worst-case โดยตัวมันเอง
3. average-case ไม่เท่ากับ amortized
4. nested loops ไม่ได้เป็น n² เสมอ
5. อย่าตัด exact counts ก่อนเข้าใจ loop bounds
6. อย่าบีบ V,E เป็น n ถ้าทำให้ข้อมูลสำคัญหาย
7. recursion ใช้ call-stack space ได้แม้ไม่มี malloc
8. same Θ class ไม่ได้แปลว่า same practical speed
9. hash lookup ไม่ใช่ unconditional O(1); assumptions สำคัญ
10. benchmark input เล็กไม่ใช่ proof ของ asymptotic class
