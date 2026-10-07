# Chapter 121 — Merkle Patricia Trie

บทนี้รวม 3 แนวคิด:

- radix trie บน nibble (ฐาน 16)
- Patricia path compression
- SHA-256 authenticated node hashing

Key มีขนาดคงที่ **32 bytes = 64 nibbles** และ value เป็น `uint64_t`.

Node มี 3 ชนิด:
- Leaf — path suffix + value
- Extension — compressed path + child
- Branch — children 16 ทาง

## Canonical Root

Root hash ไม่ขึ้นกับลำดับ insertion. ทุก node hash ใช้ domain tag และ deterministic encoding:

- Leaf: `H(0x20 || path_len || path || value)`
- Extension: `H(0x21 || path_len || path || child_hash)`
- Branch: `H(0x22 || child_hash[0] ... child_hash[15])`

Missing branch child ใช้ 32 zero bytesใน encoding.

## Path Compression

ถ้า keys มี prefix ร่วมกันหลาย nibbles จะเก็บ prefix ครั้งเดียวใน Extension node แทน chain ของ single-child branches.

## Tests

- 10,000 deterministic 32-byte keys
- insertion order A และ shuffled order B ต้องได้ root เดียวกัน
- exact lookup ทุก key
- update 1,000 keys
- root changes after updates
- missing-key lookup
- structural/hash validation
- path-compression statistics
- ASan/UBSan

## Complexity

Lookup/insert จำกัดด้วย 64 nibblesของ key จึง O(64) = O(1) เมื่อ key width คงที่ แต่ node allocation/splitting ยังมี constant factors ตาม Patricia structure.
