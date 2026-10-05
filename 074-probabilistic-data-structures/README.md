# Chapter 074 — Probabilistic Data Structures

Probabilistic Data Structures deliberately trade exactness for lower memory, higher throughput, mergeability or streaming operation.

Core questions before choosing one:
- What error type is allowed: false positive, false negative, or numeric estimation error?
- Is error one-sided or two-sided?
- Is the guarantee probabilistic, expected, or empirical?
- How does memory change error?
- Can sketches merge?
- What assumptions are made about hashing/randomness/adversarial inputs?

This chapter uses **KMV (K-Minimum Values)** as a concrete representative sketch. It hashes each distinct value into the 64-bit universe and retains only the k smallest distinct hashes.

If fewer than k distinct hashes have been observed, the count is exact. Once k are retained, the kth-smallest order statistic estimates cardinality:

    n_hat ≈ (k - 1) * 2^64 / R_k

where R_k is the largest retained hash (the kth-smallest).

Larger k uses more memory and usually reduces estimator variance. Mergeability is natural: the merged KMV sketch is simply the k smallest distinct hashes from both sketches.

Teaching implementation keeps an unsorted k-array and scans it, so update is O(k), not a production-optimized O(log k) heap/tree design. This is intentional to expose estimator semantics before optimization.

Topics surveyed: Bloom/Counting Bloom/Cuckoo filters, cardinality sketches, frequency sketches, sampling sketches, mergeability, confidence/error, hash assumptions and adversarial robustness.
