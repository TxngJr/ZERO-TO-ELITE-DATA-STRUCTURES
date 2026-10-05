# Chapter 076 — Count-Min Sketch

Count-Min Sketch (CMS) estimates nonnegative stream frequencies with a depth x width counter matrix.

Each row hashes a key to one counter. Update by delta adds delta to one counter per row. Query returns the minimum counter across rows.

Because collisions only add nonnegative mass, the estimate never underestimates the true frequency under the nonnegative-update model:

    f_hat(x) >= f(x)

The overestimate comes from collision noise.

Classical parameterization uses width about e/epsilon and depth about ln(1/delta) to obtain an additive-error probability guarantee under suitable hashing assumptions.

This implementation uses deterministic mixed row seeds for repeatable education/tests. It does not claim formal pairwise-independent hash-family proof.

Updates precheck every touched counter and total_weight before mutation, so overflow failure is transactional. Merge requires equal width/depth and identical deterministic seed scheme.

Complexity: add/query O(depth), merge O(width*depth), storage O(width*depth).
