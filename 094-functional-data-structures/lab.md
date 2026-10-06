# Lab — Functional Queue History

สร้าง q0→q1(10)→q2(20), dequeue q2 เป็น q3 และแตก branch จาก q1 ด้วย enqueue 99. ตรวจ q2 ยังเป็น [10,20], q3=[20], branch=[10,99].

```bash
cmake -S . -B build
cmake --build build --target ch094_functional_tests ch094_functional_benchmark
ctest --test-dir build -R ch094 --output-on-failure
```

Debugging: ใส่ full validator ในทุก enqueue แล้ว benchmark 200k operations เพื่อสังเกต hidden O(n).
