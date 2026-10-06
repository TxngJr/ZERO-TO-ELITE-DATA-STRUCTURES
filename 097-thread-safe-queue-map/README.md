# Chapter 097 — Thread-Safe Queue / Map

บทนี้เปลี่ยนจาก generic concurrent set ของ Chapter 096 มาเป็นโครงสร้างที่มี concurrency contract ชัดขึ้นสองแบบ:

- `ThreadSafeQueue` — bounded MPMC ring queue ใช้ mutex + condition variables
- `ThreadSafeIntMap` — fixed-bucket striped hash map ใช้ mutex แยกต่อ bucket

เป้าหมายคือเข้าใจว่า “thread-safe” ไม่ได้แปลว่า lock-free และไม่ใช่เพียงการใส่ mutex รอบ function ทุกตัวแบบสุ่ม ๆ แต่ต้องกำหนด invariant, atomic operation boundary, blocking semantics และ lifetime.

## Queue Contract

Logical ring:

```text
data[capacity]
head -> oldest
tail -> next write
size in [0,capacity]
```

Operations:
- `try_push/try_pop` — lock แต่ไม่ wait
- `push_wait/pop_wait` — wait ด้วย condition variable
- `close` — ห้าม push ใหม่, wake waiters, consumers drain ของเดิมก่อนจบ

Condition wait ต้องอยู่ใน `while` เพราะ wake-up ไม่ได้พิสูจน์ predicate เอง.

## Map Contract

Map แบ่ง key space เป็น buckets:

```text
hash(key) -> bucket i -> mutex_i -> linked chain
```

Keys ใน bucket ต่างกันทำงานขนานกันได้มากกว่าการใช้ global mutex เดียว. `size` มี mutex แยกต่างหาก.

## Correctness

Queue linearization เกิดขณะถือ queue mutex เมื่อ slot/metadata ถูก publish.
Map put/get/remove linearize ขณะถือ bucket mutex ของ key นั้น.

## Complexity

Queue try/wait operation มี O(1) sequential work แต่ wait time ไม่ bounded.
Map expected lookup/update ขึ้นกับ chain length; fixed bucket count ทำให้ worst case O(n).

## Lifetime

`free` ใช้ได้เมื่อ worker threads หยุดหมดแล้วเท่านั้น. Thread safety ของ operations ไม่ได้ทำให้ destruction ระหว่างมี users ปลอดภัยอัตโนมัติ.

## Build

```bash
cmake -S . -B build-tsan -DDS_ENABLE_THREAD_SANITIZER=ON
cmake --build build-tsan --target ch097_thread_safe_tests
ctest --test-dir build-tsan -R "^ch097_" --output-on-failure
```
