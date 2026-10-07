# Chapter 120 — Merkle Tree

บทนี้เริ่มหมวด Cryptographic / Distributed Structures ด้วย **binary Merkle tree using SHA-256**.

รองรับ:
- SHA-256 implementation จาก algorithm specification
- leaf hashing with domain prefix `0x00`
- internal-node hashing with domain prefix `0x01`
- odd-node duplication
- root extraction
- inclusion proof generation
- inclusion proof verification
- full tree validator

## Domain Separation

Leaf:

```text
H(0x00 || leaf_bytes)
```

Internal:

```text
H(0x01 || left_hash || right_hash)
```

Prefix แยก namespace ของ leaf และ internal node ลด ambiguity ระหว่าง raw leaf bytes กับ concatenated child hashes.

## Odd Level

ถ้าระดับหนึ่งมี node คี่ ตัวสุดท้ายถูกจับคู่กับตัวเอง:

```text
A B C
| | |
H(A,B) H(C,C)
```

Proof จึงยังมี sibling hash ทุกระดับ.

## Inclusion Proof

Proof เก็บ:
- sibling hash ต่อระดับ
- flag ว่า sibling อยู่ซ้ายหรือขวา

Verifier hash leaf แล้ว fold ขึ้นทีละระดับจนได้ candidate root.

## Tests

- SHA-256 known vector `"abc"`
- Merkle tree 4,097 leaves × 32 bytes
- full invariant validation
- inclusion proofs every 37th leaf
- tampered leaf rejection
- root changes when one leaf changes
- single-leaf proof
- ASan/UBSan

## Complexity

Build O(N log-free hashing work) = O(total leaf bytes + N hash blocks).
Proof size/verification O(log N).
