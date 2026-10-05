# Pitfalls — Rolling Hash

- treating equal hashes as mathematical proof
- forgetting byte+1 mapping
- mixing inclusive and half-open ranges
- using a multiplication that overflows chosen integer width
- using one weak modulus/base without discussing collision risk
- mutating text after prefix construction
- reporting Rabin-Karp hash candidates without verification
- claiming deterministic O(n+m) when verification can degenerate
