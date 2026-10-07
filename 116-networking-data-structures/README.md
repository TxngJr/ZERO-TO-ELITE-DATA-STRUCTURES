# Chapter 116 — Networking Data Structures

บทนี้สร้างสองโครงสร้างที่พบใน networking stack:

1. **IPv4 Longest-Prefix-Match Route Trie**
2. **Flow Table** แบบ open addressing

## Route Trie

Prefix ถูกเดินจาก bit สำคัญสุดไปต่ำสุด:

```text
root
 ├─0
 └─1
    └─0 ...
```

ทุก node อาจมี route หรือเป็นเพียง path node. Lookup เดิน address 32 bits และจำ route ล่าสุดที่ match จึงได้ longest-prefix match.

รองรับ prefix length 0..32 รวม default route `0/0`.

## Flow Table

Flow key ใช้:
- source IPv4
- destination IPv4
- source port
- destination port
- protocol

Table ใช้ power-of-two capacity, linear probing, tombstones และ rehash เมื่อ occupied+tombstone load เข้าใกล้ 0.7.

## Tests

- default /0, /8, /16, /24 precedence
- 4,096 distinct /24 routes
- exact longest-prefix verification
- 50,000 flow insertions
- full lookup verification
- updates without duplicate insertion
- remove half the flows
- tombstone/size/capacity validation
- ASan/UBSan

## Complexity

Route lookup bounded by IPv4 width: O(32) = O(1).
Route insertion O(prefix length) <= 32.
Flow operations expected O(1), worst O(capacity) under clustering.
