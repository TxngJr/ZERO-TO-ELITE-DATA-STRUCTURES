# Chapter 027 — Binomial Heap

## Goal

Binomial Heap ออกแบบให้ meld/union เป็น operation สำคัญ โดยเก็บ forest ของ Binomial Trees ที่ degree ไม่ซ้ำ คล้าย binary representation ของจำนวน nodes.

หลังจบบทนี้คุณควร:
- นิยาม Binomial Tree B_k
- เข้าใจ root-list degrees
- link trees degree เท่ากัน
- merge root lists
- consolidate duplicates
- insert
- peek/extract minimum
- destructive meld
- decrease-key ด้วย node handle
- delete handle
- validate binomial shape + heap order

## Binomial Tree

B_0 มีหนึ่ง node.

B_k สร้างจาก B_(k-1) สองต้นโดย link root หนึ่งเป็น child ของอีก root.

Properties:
- nodes = 2^k
- root degree = k
- height = k

## Forest Representation

Root degrees เรียงเพิ่มและไม่ซ้ำ.

ตัวอย่าง size=13:

    13 = 8 + 4 + 1

จึงมี roots degrees:

    0,2,3

เหมือน set bits ของ 1101 base 2.

## Heap Order

Min Binomial Heap:

    parent.key <= child.key

Minimum อยู่ที่ root ใด root หนึ่ง.

Implementation นี้ไม่ cache min pointer ดังนั้น peek-min scan root list:

    O(log n)

## Linking Equal-Degree Trees

ถ้า roots x,y degree เท่ากัน:
- root key เล็กกว่าเป็น parent
- อีก root ถูก prepend เป็น child
- parent degree เพิ่มหนึ่ง

นี่คล้าย carry ของ binary addition.

## Merge Root Lists

รวม root lists สองชุดให้เรียง degree แบบ merge sorted linked lists.

จำนวน roots ต่อ heap:

    O(log n)

จึง merge ได้ logarithmic.

## Consolidation

หลัง merge อาจมี roots degree ซ้ำ.

Walk root list:
- degree ต่าง -> advance
- ถ้ามีสาม roots degree เดียวกัน ต้อง delay การ link คู่แรก
- ถ้ามีสอง roots degree เดียวกัน -> link ตาม key

ผลสุดท้ายกลับมามี degree ไม่ซ้ำ.

## Meld

Destructive meld:

    meld(destination, source)

หลังสำเร็จ:
- destination ถือ nodes ทั้งหมด
- source ว่าง

ไม่ copy nodes.

Time:

    O(log(n+m))

## Insert

สร้าง B_0 แล้ว union เข้ากับ forest.

Worst-case:

    O(log n)

เพราะอาจเกิด chain of carries.

## Extract Minimum

1. scan roots หา minimum
2. remove min root
3. children ของ min root อยู่ degree descending
4. reverse child list ให้ degree ascending
5. set promoted child parent=NULL
6. union กับ remaining roots
7. free old min root

Time:

    O(log n)

## Child Degree Order

เมื่อ link child เข้า parent จะ prepend.

ดังนั้น children ของ B_k root อยู่ degree:

    k-1,k-2,...,0

Validator ตรวจ property นี้ตรง ๆ.

## Decrease-Key

decrease-key ด้วย live node handle:
- require new_key <= old key
- set key
- ขณะที่ parent key ใหญ่กว่า ให้ swap integer key payload ขึ้น

Time:

    O(log n)

### Handle Semantics

Implementation swap key payloads ไม่ได้ย้าย physical nodes.

ดังนั้น handle เป็น pointer ไป storage node ไม่ใช่ stable logical-item identity.

หลัง bubbling handle เดิมอาจเก็บ key คนละค่ากับ logical item ที่ลด key.

## Delete by Handle

Teaching implementation bubble target key payload ไป root ของ binomial tree แล้ว remove root นั้น.

Precondition:
handle ต้องเป็น live node ของ heap นี้.

เราไม่เก็บ owner pointer ในทุก node เพราะ destructive meld จะต้องแก้ owner metadata จำนวนมากถ้าออกแบบแบบตรงไปตรงมา.

## Duplicates

Duplicate integer keys allowed.

Heap invariant uses <=.

## Validation

Validator checks:
- root parent=NULL
- root degrees strictly increasing
- each B_k child list degree exactly k-1...0
- parent <= child
- total reachable count=size
- traversal count never exceeds size

## Complexity

| Operation | Complexity |
|---|---:|
| peek-min | O(log n) |
| insert | O(log n) |
| extract-min | O(log n) |
| meld | O(log(n+m)) |
| decrease-key(handle) | O(log n) |
| delete(handle) | O(log n), valid-handle precondition |
| validate | Theta(n) |
| storage | Theta(n) |

## Binary Heap vs Binomial Heap

Binary Heap:
- compact array
- strong locality
- peek O(1)
- BUILD-HEAP Theta(n)

Binomial Heap:
- pointer forest
- weaker locality
- peek O(log n) here
- efficient meld
- handle-oriented decrease operations

No universal winner.
