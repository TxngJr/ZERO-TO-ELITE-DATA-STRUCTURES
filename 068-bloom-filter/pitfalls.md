# Pitfalls — Bloom Filter

- treating positive as proof of membership
- deleting by clearing bits
- choosing m/k without workload estimates
- counting duplicate inserts as distinct population
- weak/degenerate hash derivation
- forgetting binary/empty-key semantics
- claiming measured false-positive rate is a universal guarantee
