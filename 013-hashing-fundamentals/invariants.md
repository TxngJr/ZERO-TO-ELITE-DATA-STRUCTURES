# Invariants and Contracts — Hashing

1. Equal keys must hash equal.
2. Hash collisions are allowed.
3. Bucket index must remain in [0,m).
4. Byte hashing must process exactly the documented byte range.
5. Runtime hash stability must not be assumed unless documented.
6. Security-sensitive hashing requires a different threat model.

## Equality consistency

If custom equality treats "Cat" and "cat" as equal, the custom hash must normalize/mix consistently so both get equal hashes.

Violating this makes a Hash Map logically incorrect, not merely slower.
