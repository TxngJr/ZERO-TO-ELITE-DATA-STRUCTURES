# Chapter 122 — Blockchain Data Structures

บทนี้สร้าง immutable teaching blockchain ที่เชื่อม:

- canonical transaction records
- transaction Merkle root
- block header hash
- previous-block hash links
- transaction inclusion proofs
- full-chain validation

## Transaction

`BcTransaction` มี fields คงที่:

- from
- to
- amount
- nonce

ทุก field เป็น `uint64_t` และ serialize เป็น big-endian 32 bytes ก่อนเข้า Merkle tree จึงไม่พึ่ง C struct padding หรือ host endianness.

## Block Commitment

แต่ละ block เก็บ:

- height
- timestamp
- previous block hash
- transaction Merkle root
- transaction count
- block hash

Block hash:

```text
SHA256(
  0x30 ||
  height ||
  previous_hash ||
  tx_root ||
  timestamp ||
  tx_count
)
```

## Immutability

`bc_append` copy transactions เข้า block-owned storage. การแก้ input array หลัง append จึงไม่เปลี่ยน chain.

## Tests

- 200 blocks × 64 transactions
- exact height/prev-hash linkage
- full-chain recomputation
- transaction inclusion proofs
- tampered transaction proof rejection
- caller-buffer mutation does not affect stored block
- ASan/UBSan

## Scope

นี่เป็น data-structure model ไม่ใช่ cryptocurrency protocol:
- ไม่มี Proof-of-Work/Proof-of-Stake
- ไม่มี signatures
- ไม่มี mempool
- ไม่มี fork-choice rule
- ไม่มี networking/consensus
