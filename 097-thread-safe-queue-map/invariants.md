# Invariants

Queue:
1. `0 <= size <= capacity`.
2. `head,tail < capacity`.
3. `tail == (head + size) mod capacity` in quiescent state.
4. closed queue accepts no new pushes.
5. closed queue may still be drained.

Map:
6. every node belongs to exactly one hash bucket.
7. no duplicate key exists in one bucket.
8. size equals total nodes in quiescent validation.
9. bucket chains are accessed only while holding that bucket mutex.
10. destruction requires no live users.
