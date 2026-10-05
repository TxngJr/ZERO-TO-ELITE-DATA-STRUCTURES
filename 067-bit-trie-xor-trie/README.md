# Chapter 067 — Bit Trie / XOR Trie

## Goal

Bit Trie แทน unsigned integers เป็นเส้นทาง bit จาก MSB ไป LSB:

    bit 63 -> bit 62 -> ... -> bit 0

บทนี้ implement multiset XOR Trie สำหรับ uint64_t รองรับ insert, remove one occurrence, contains, min/max XOR partner, count ของค่าที่ทำให้ (value XOR query) < limit และ structural validation.

## Node Metadata

Each node stores child[0], child[1], subtree_count และ terminal_count.

`subtree_count` นับ live values รวม duplicates ใน prefix subtree ส่วน `terminal_count` อยู่ที่ depth 64 และเก็บ multiplicity ของ exact value.

## Greedy XOR

Maximum XOR เลือก branch ตรงข้าม query bit เมื่อ branch นั้นยัง live เพราะ XOR bit 1 ที่ตำแหน่งสูงกว่าจะชนะ lower bits ทั้งหมด. Minimum XOR เลือก same bit เพื่อสร้าง XOR bit 0 เมื่อทำได้.

## Duplicates and Removal

โครงสร้างเป็น multiset. Insert ค่าเดิมหลายครั้งใช้ path เดิมและเพิ่ม counts. Remove ลบครั้งละหนึ่ง occurrence. บทนี้ไม่ reclaim historical nodes หลัง count ลดเป็นศูนย์; query จะ ignore child ที่ subtree_count == 0.

## Transactional Insert

uint64_t path ต้องสร้างเพิ่มได้มากสุด 64 nodes. Insert reserve current_node_count + 64 ก่อน mutate counts/links ดังนั้น arena growth ไม่ล้มกลาง path publication.

## Count XOR < Limit

เดิน MSB-first. ถ้า limit bit เป็น 0 ต้องเดิน branch ที่ทำ XOR bit 0. ถ้า limit bit เป็น 1 สามารถบวก subtree ของ branch ที่ทำ XOR bit 0 ได้ทั้งหมด แล้วเดิน branch XOR bit 1 ต่อ.

## Complexity

For fixed uint64_t ทุก operation หลักใช้ 64 levels = O(1) เมื่อ width คงที่. สำหรับ w-bit keys เป็น O(w).

## Files

- src/int_xor_trie.*
- tests/test_xor_trie.c
- examples/xor_trie_demo.c
- benchmarks/xor_trie_benchmark.c
