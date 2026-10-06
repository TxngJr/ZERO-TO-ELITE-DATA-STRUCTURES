# Exercises

## Beginner
1. Trace refs หลัง clone x เป็น y,z.
2. Trace set(y) เมื่อ refs=3.
3. Trace set(y) เมื่อ refs=1.
4. COW vs deep copy.
5. COW vs immutable.

## Intermediate
6. เพิ่ม reserve.
7. เพิ่ม insert(index,value).
8. Test 100 read-only clones.
9. Fault-injection detach.
10. วัด clone latency หลาย payload sizes.

## Advanced
11. วิเคราะห์ peak memory เมื่อ clones mutate.
12. ออกแบบ small-buffer + COW.
13. เปรียบเทียบ persistent vector.
14. วิเคราะห์ cache/TLB detach cost.
15. อธิบาย why atomic refcount alone is insufficient.

## Implementation
16. เพิ่ม COW byte string.
17. เพิ่ม shrink policy.
18. เพิ่ม allocation counter.

## Challenge / Research
19. เปรียบเทียบ page-level vs vector-level COW.
20. หา crossover point ของ read-heavy/write-heavy workloads.
