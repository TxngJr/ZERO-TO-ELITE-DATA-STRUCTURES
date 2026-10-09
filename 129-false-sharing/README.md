# Chapter 129 — False Sharing

False sharingเกิดเมื่อหลาย threadsเขียน **คนละ logical variables** แต่ variablesเหล่านั้นอยู่ใน cache lineเดียวกัน.

ไม่มี data raceในภาษา C จำเป็นต้องเกิดขึ้นเลยก็ได้. ถ้าตัวแปรเป็น atomicsและแต่ละ threadเขียนของตัวเอง โปรแกรมยัง data-race-free แต่ cache-coherence trafficอาจสูงเพราะ line ownershipเด้งข้าม cores.

บทนี้มี 2 ส่วน:

1. **Cache-line layout model**
2. **Threaded atomic counter workload**

## Counter Array

Counterแต่ละตัวเป็น `_Atomic uint64_t`.

Arrayกำหนด:
- counter count
- stride
- cache-line size

Base addressถูก alignตาม cache-line boundary.

Packed example:

```text
line 0:
counter0 counter1 counter2 ... counter7
```

เมื่อ stride=8 และ line=64.

Padded example:

```text
counter0 at line 0
counter1 at line 1
counter2 at line 2
...
```

เมื่อ stride=64.

## Deterministic Checks

Tests assert:
- packed counters share lines
- padded countersไม่ share line
- line occupancyถูกต้อง
- parallel relaxed-atomic incrementsได้ exact final counts
- reset works
- layout metadata valid
- TSan sees no data race

## Benchmark

Benchmarkเปรียบเทียบ packed vs line-separated countersด้วยหลาย threads แต่ **ไม่ assert timing** เพราะ scheduler, CPU topology, virtualization และ cache coherence implementationต่างกัน.
