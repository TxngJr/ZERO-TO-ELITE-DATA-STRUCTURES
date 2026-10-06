# Chapter 104 — Arena Allocator

Arena allocator เหมาะกับงานที่มี **many allocations + shared lifetime**. แทนที่จะ free ทีละ object, arena ใช้ bump pointer ภายใน block และ reclaim เป็นกลุ่มด้วย mark/reset หรือ reset ทั้ง arena.

Implementation นี้เป็น growable block arena:
- default block size
- bump allocation
- power-of-two alignment
- alignment สูงสุด 4096 bytes
- grow ด้วย block ใหม่เมื่อ head block ไม่พอ
- `arena_mark`
- `arena_reset_to_mark`
- `arena_reset`
- `arena_calloc`
- overflow-safe size/alignment arithmetic
- generation token ป้องกัน reuse marker หลัง reset

## Mark / Reset

Mark เก็บ current block identity, used offset, total used, block count และ generation.

Reset-to-mark จะ free blocks ที่สร้างหลัง mark, restore used offset และ increment generation เพื่อ invalidate marker เก่า.

## Tests

- 50,000 allocations
- sizes 1..97
- alignment 1..4096
- multi-block growth
- calloc zero verification
- mark/reset + stale-mark rejection
- full reset เหลือ base block
- ASan/UBSan + LeakSanitizer

## Complexity

| Operation | Time |
|---|---:|
| fast alloc | Θ(1) |
| grow alloc | Θ(1) + system allocation |
| mark | Θ(1) |
| reset-to-mark | O(newer blocks) |
| full reset | Θ(block count) |
| validate | Θ(block count) |
