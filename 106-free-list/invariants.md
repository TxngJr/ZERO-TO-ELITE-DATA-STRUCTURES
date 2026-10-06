# Invariants

1. managed capacity > 0.
2. free extents อยู่ใน range.
3. allocation extents อยู่ใน range.
4. free list sorted strictly by offset.
5. free extents ไม่ overlap และไม่ adjacent.
6. allocations ไม่ overlap กัน.
7. allocations ไม่ overlap free extents.
8. sum(free sizes)+sum(allocation sizes)=capacity.
9. tracked allocated bytes ตรง sum allocation sizes.
10. allocation count ตรง allocation-list length.
