# Implementation Notes

`FsCounterArray` over-allocates and manually aligns its base to the requested cache-line size, retaining the raw pointer for `free`.

Requirements:
- line size is a power of two
- line size >= alignment of `_Atomic uint64_t`
- stride >= sizeof atomic counter
- stride is a multiple of atomic alignment
- count × stride and allocator slack are overflow checked

Each counter is initialized with `atomic_init`.

`fs_parallel_increment`:
- creates one pthread per selected counter
- each thread touches only its assigned atomic
- uses `memory_order_relaxed` because counters have no inter-counter ordering requirement
- joins all created threads before returning

The layout helper maps pointer addresses to line numbers relative to aligned base, giving deterministic false-sharing structure independent of real physical cache indexing.
