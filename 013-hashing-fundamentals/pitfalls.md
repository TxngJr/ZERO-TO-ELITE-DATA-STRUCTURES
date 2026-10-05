# Pitfalls — Hashing

- treating hash equality as key equality
- using raw key % bucket_count on patterned integers without analysis
- using a cryptographic hash everywhere despite unnecessary cost
- using a weak table hash for passwords/security
- forgetting string-hash Θ(length)
- changing equality without changing hash contract
- assuming runtime hashes stable across versions/processes
- judging hash quality from one tiny sample
- using power-of-two masking with weak low bits
- ignoring adversarial collision inputs
- calling expected O(1) a worst-case guarantee
