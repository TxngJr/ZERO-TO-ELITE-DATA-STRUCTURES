# Chapter 119 — Game Data Structures

บทนี้สร้าง teaching model ของ data structures ที่พบบ่อยในเกม:

1. **Generation-Safe Entity Pool**
2. **Sparse-Set Position Component**
3. **Uniform Spatial Grid**

## Entity Handle

Entity ID pack:
```text
high 32 bits = generation
low  32 bits = slot
```

เมื่อ entity ถูก destroy แล้ว slot reuse, generation เปลี่ยน ทำให้ stale handle ใช้ไม่ได้.

## Sparse Set

Position component ใช้:
- dense entity-slot array
- dense x/y arrays
- sparse[slot] -> dense index

Add/get/remove จึง O(1) และ dense arrays เหมาะกับ iteration แบบ cache-friendly.

Remove ใช้ swap-remove:
```text
remove dense[i]
move last -> i
update sparse[moved_slot] = i
```

## Uniform Spatial Grid

Grid rebuild จาก current Position set:
- one linked chain per cell
- next pointer indexed by entity slot
- query AABB scans only overlapping cells
- exact coordinate filter avoids false positives from cell overlap

Grid stores `world_version`. ถ้า world mutate หลัง rebuild, query ถูก reject จนกว่าจะ rebuild ใหม่ จึงไม่ silently return stale spatial results.

## Tests

- 50,000 entities
- 50,000 position components
- generation-safe stale-handle rejection
- sparse-set swap-remove validation
- spatial grid 100×20 cells
- exact AABB result count 1,250
- stale-grid detection after mutation
- rebuild + full invariant validation
- ASan/UBSan

## Complexity

Entity spawn/destroy and component access are O(1) except grid rebuild O(P).
AABB query is O(overlapping cells + candidates).
