# Implementation Notes

`loc_analyze_indices` accepts:
- index array
- element size
- cache-line size

It performs overflow-safe `index * element_size` before converting to line IDs.

Unique-line/first-touch tracking uses an open-addressed hash set sized from access count. The set stores line IDs and an explicit used bit, so line 0 is valid.

No real hardware counters are used. Results are deterministic for the same trace.

Generator helpers create:
- sequential traces
- modular strided permutations when `gcd(stride,count)=1`

The strided permutation guard prevents accidental short cycles.
