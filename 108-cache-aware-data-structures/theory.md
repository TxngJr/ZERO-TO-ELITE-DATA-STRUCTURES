# Theory — Cache Awareness

CPU cache ทำงานเป็น cache lines/sets/levels. Data structure ที่ cache-aware เลือก layout/block size โดยรู้ parameter บางอย่างของ memory hierarchy.

หลักสำคัญ:
- spatial locality
- temporal locality
- working set
- cache line
- blocking / tiling
- structure-of-arrays vs array-of-structures
- pointer chasing
- prefetchability

Blocked matrix แปลง neighborhood 2D ให้ physical data ใกล้กันใน tile. Algorithm ที่ process tile-by-tile จึงมีโอกาส reuse cache lines ก่อน eviction.

Cache-aware ต่างจาก cache-oblivious Chapter 109: cache-aware ใช้ block/tile parameter explicit; cache-oblivious ออกแบบ recursion/layout โดยไม่ hard-code cache size.
