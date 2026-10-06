# Chapter 107 — Garbage-Collected Data Structures

บทนี้สร้าง exact tracing **mark-sweep heap** สำหรับ object graph เพื่อให้เห็นว่า GC data structure ต้องเก็บทั้ง object state, roots, references, mark state และ free-slot state.

Implementation:
- fixed-capacity managed heap
- 64-bit generation-based handles
- 2 outgoing references ต่อ object
- explicit root set
- iterative mark stack
- sweep unreachable objects
- free-slot reuse
- stale-handle rejection
- slot retirement แทน generation wrap

## Handle Model

```text
GcHandle = { index, generation }
```

เมื่อ object ถูก sweep:
1. ถ้า generation ยังเพิ่มได้ → increment แล้ว slot กลับ free stack
2. ถ้า generation = `UINT64_MAX` → retire slot ถาวรแทนการ wrap
3. handle เก่าจึงไม่กลับมา valid จาก counter wrap

## Mark-Sweep

Mark เริ่มจาก roots แล้ว trace references ด้วย preallocated stack. Sweep reclaim alive+unmarked objects.

Unreachable cycle ถูก collect ได้ เพราะ reachability เริ่มจาก roots ไม่ใช่ reference count.

## Tests

- capacity 4,096
- allocate 3,000-node random graph
- explicit unreachable cycle
- random roots
- independent reference reachability calculation
- compare every handle after collect
- remove subset of roots and collect again
- stale-handle rejection after slot reuse
- ASan/UBSan + LeakSanitizer

## Scope

นี่เป็น exact tracing simulator:
- references ต้องผ่าน GC handle fields ที่ implementation รู้จัก
- ไม่มี conservative stack scanning
- ไม่มี moving/compacting
- ไม่มี generational young/old collector
- ไม่ thread-safe
