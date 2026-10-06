# Chapter 094 — Functional Data Structures

Functional Data Structures ใช้แนวคิด **pure transformation**: operation รับ structure เดิมแล้วคืน structure ใหม่ โดยไม่ mutate input เดิม. บทนี้ต่อจาก Persistent/Immutable Structures แต่เน้นวิธีคิดแบบ functional, referential transparency, immutable links, structural sharing และ amortized reasoning.

Implementation หลักคือ `FunctionalQueue` แบบ two immutable lists:

```text
logical queue = front ++ reverse(rear)
```

- enqueue: cons ค่าใหม่เข้า rear — Θ(1)
- dequeue: ใช้ front ถ้ามี; ถ้า front ว่าง reverse-copy rear เป็น front
- dequeue worst case Θ(r), แต่ amortized Θ(1) บน **linear history**
- branching จาก historical version เดิมอาจทำให้ normalization เดิมเกิดซ้ำ จึงต้องระบุ workload model ก่อนอ้าง amortized bound

## Learning Objectives

ผู้เรียนต้องสามารถอธิบาย pure function, referential transparency, structural sharing, FIFO invariant, strict vs lazy functional queues, arena lifetime และเหตุผลที่ expensive validator ไม่ควรอยู่ใน hot path.

## Mental Model

```text
front: 10 -> 20 -> NULL
rear : 40 -> 30 -> NULL

logical: 10,20,30,40
```

enqueue(50) สร้าง node ใหม่ `50 -> old rear`; old queue ยังใช้งานได้.

เมื่อ front ว่าง:

```text
rear 40 -> 30 -> 20
      reverse-copy
front 20 -> 30 -> 40
```

## Correctness

Representation invariant:
- front NULL iff front_len=0
- rear NULL iff rear_len=0
- actual list lengths ตรง metadata
- logical sequence = front ++ reverse(rear)
- published nodes ไม่ถูกแก้

Enqueue รักษา FIFO เพราะค่าที่ cons ด้านหน้า rear จะไปอยู่ท้ายสุดหลัง reverse. Dequeue จาก normalized front จึงคืน element ที่เก่าสุดเสมอ.

## Memory

Nodes อยู่ใน fixed-block arena เพื่อให้ address เสถียร. ทุก historical queue valid จน `fq_arena_free`; implementation นี้ไม่ได้ reclaim ราย version.

## Complexity

| Operation | Worst | Amortized on linear history |
|---|---:|---:|
| enqueue | Θ(1) | Θ(1) |
| peek/dequeue | Θ(r) | Θ(1) |
| size | Θ(1) | Θ(1) |
| full validate | Θ(n) | N/A |

## When NOT to Use

ไม่เหมาะเมื่อ branching สูงมากจน normalization ซ้ำ, ต้อง reclaim versions แยกกัน, หรือต้องการ locality/constant factor ของ mutable ring buffer.

## Build

```bash
cmake -S . -B build
cmake --build build --target ch094_functional_tests ch094_functional_benchmark
ctest --test-dir build -R ch094 --output-on-failure
```
