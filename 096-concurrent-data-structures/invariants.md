# Invariants

Representation:
1. live set has non-NULL data.
2. capacity >= 8.
3. size <= capacity.
4. data[0..size) strictly increasing.
5. no duplicates.

Synchronization:
6. all data/size/capacity accesses happen while holding mutex.
7. realloc and pointer publication happen under mutex.
8. snapshot copies one consistent locked state.
9. destruction only after all users stop.

Atomic demo:
10. ready is atomic.
11. producer uses release store after payload write.
12. consumer uses acquire load before payload read.
