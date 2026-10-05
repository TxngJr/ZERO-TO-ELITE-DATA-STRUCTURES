# Chapter 078 — Reservoir Sampling Structures

Reservoir Sampling keeps a uniform sample of fixed size k from a stream whose final length may be unknown.

Algorithm R:
1. Fill the reservoir with the first k items.
2. For the t-th subsequent item (1-based stream count t), draw j uniformly from [0,t).
3. If j < k, replace reservoir[j]; otherwise discard the new item.

After processing n >= k items, every stream position has inclusion probability k/n.

This implementation uses xorshift64* state plus rejection-based bounded sampling rather than naive modulo reduction. The rejection step removes modulo bias for arbitrary bounds.

Seeded RNG makes tests and experiments reproducible; reproducibility is not a claim of cryptographic randomness.

Storage is O(k), each update is expected O(1), and no original stream replay is required.

Reservoir Sampling is different from cardinality/frequency sketches: it preserves actual sampled items, not just aggregate estimates.
