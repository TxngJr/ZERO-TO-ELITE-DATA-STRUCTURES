# Pitfalls — Skip List

- claiming worst-case O(log n)
- confusing probabilistic guarantee with invariant
- forgetting update[] predecessor array
- unlinking node only at level 0
- leaving current_level too high after deletion
- allocating max-level pointers for every node unnecessarily
- bad PRNG/adversarial correlation changing expected behavior
- treating deterministic test seed as cryptographic randomness
- range scanning upper levels instead of ordered level 0
