# Chapter 014 — Hash Table

## Goal

Hash Table นำ hash function จาก Chapter 013 มารวมกับ bucket array, collision resolution และ resizing เพื่อสร้าง key→value lookup structure.

บทนี้ implement Int→Int Hash Table แบบ Separate Chaining.

หลังจบบทนี้คุณควร:
- อธิบาย bucket array + chains
- implement put/get/remove
- แยก insert ใหม่กับ update key เดิม
- เข้าใจ collision handling
- รักษา size และ load factor
- implement rehash/resize
- วิเคราะห์ expected vs worst-case complexity
- เข้าใจ why resize must recompute bucket index
- เขียน invariant validator
- ทำ randomized differential tests
- เปรียบเทียบ chaining กับ open addressing conceptually

## 1. Representation

Table:

    buckets
      |
      v
    [0] -> (k=10,v=7) -> (k=26,v=9)
    [1] -> NULL
    [2] -> (k=5,v=1)
    [3] -> ...

แต่ละ bucket เป็น singly linked chain.

Different keys สามารถอยู่ chain เดียวกันได้.

## 2. Lookup

1. hash key
2. reduce to bucket index
3. walk chain
4. compare actual keys
5. return value if equal

Expected chain length depends on load factor and hash distribution.

## 3. Insert vs Update

put(k,v):
- ถ้า key อยู่แล้ว: update value, size ไม่เปลี่ยน
- ถ้ายังไม่มี: allocate Entry, prepend/append into bucket, size++

นี่สำคัญต่อ Map semantics.

## 4. Remove

หา predecessor/link ของ target ใน chain แล้ว unlink.

ถ้าพบ:
- remove entry
- free node
- size--

ถ้าไม่พบ:
- table unchanged

## 5. Load Factor

    alpha = size / bucket_count

บทนี้ grow ก่อน/หลัง insert ตาม threshold 0.75.

Separate chaining สามารถ alpha>1 ได้ แต่ threshold ต่ำช่วยรักษา expected chain length สั้น.

Threshold เป็น policy ไม่ใช่ universal constant.

## 6. Rehash

เมื่อ bucket count เปลี่ยน:

    old_bucket = hash(key) % old_m
    new_bucket = hash(key) % new_m

ห้ามแค่ memcpy bucket pointers.

ต้องเดินทุก Entry และย้ายไป bucket ใหม่ตาม new_m.

การ rehash:
    Θ(n + new_bucket_count)

โดยประมาณภายใต้ cost model.

## 7. Why Hash Values May Be Cached

บาง implementations เก็บ hash ใน Entry เพื่อลดการ recompute โดยเฉพาะ string keys.

Int table นี้ hash integer ได้ถูก จึง recompute เพื่อให้ implementation อ่านง่าย.

## 8. Expected Complexity

ภายใต้ reasonable uniform hashing และ bounded load factor:

get:
    expected Θ(1)

put:
    expected Θ(1) amortized

remove:
    expected Θ(1)

Worst case:
ถ้าทุก key ชน chain เดียว:

    Θ(n)

Resize:
    Θ(n) occasional

## 9. Amortized Resizing

bucket count doubles:
8,16,32,64,...

rehash events sparse.

Over growing sequence, resize cost is amortized across many successful insertions.

แต่ expected hash-table O(1) และ amortized resize O(1) เป็นคนละ reasoning layers:
- collision behavior needs hashing assumptions
- resize behavior uses geometric growth

## 10. Separate Chaining Trade-offs

Pros:
- deletion simple
- alpha can exceed 1
- node addresses may stay stable through bucket-array resize if nodes relinked

Cons:
- allocation per entry
- pointer chasing
- extra pointer metadata
- poor locality vs open-addressing arrays

## 11. Open Addressing Preview

Open addressing เก็บ entries ใน slot array โดยตรง.

Collision:
probe slot อื่น เช่น linear/quadratic/double hashing.

Pros:
- locality
- no per-entry node allocation

Challenges:
- tombstones/deletion
- clustering
- table must keep empty slots
- high load factor hurts performance

Chapter 015 จะใช้ open addressing สำหรับอีก implementation เพื่อเปรียบเทียบ.

## 12. Hash Table Invariants

- bucket_count > 0 for live table
- buckets array allocated
- size equals total entries across all chains
- each entry resides in bucket determined by current bucket_count
- no duplicate key entries
- chains terminate; no cycles

## 13. Equality and Hash

Int key equality:
    a == b

hash:
    hash_u64_mix(bit-pattern-converted key)

Equal int keys hash equal.

For object/string maps, equality/hash consistency becomes more subtle.

## 14. API Failure Semantics

put returns false on allocation failure.

Existing table must remain valid if growth fails.

Implementation grows before allocating/inserting a new entry when threshold requires expansion, so failure leaves logical mapping unchanged.

## 15. When Hash Table Is Not Ideal

Avoid/default elsewhere when:
- ordered iteration required
- predecessor/successor/range queries needed
- worst-case deterministic guarantees required
- keys tiny and table overhead dominates
- memory locality extremely important and chaining nodes hurt workload

## Files

- src/int_int_hash_table.*
- tests/test_hash_table.c
- examples/hash_table_demo.c
- benchmarks/hash_table_benchmark.c
- theory/visual/implementation/complexity/invariants/pitfalls
- lab/exercises/quiz/references
