# Exercises

## Beginner
1. Trace lookup 10.1.2.3 สำหรับ /8,/16,/24.
2. Default route อยู่ตรงไหน?
3. Flow five-tuple มี fields อะไร?
4. Tombstone ต่างจาก EMPTY อย่างไร?
5. Route trie depth สูงสุดเท่าไร?

## Intermediate
6. เพิ่ม IPv4 route update counter.
7. เพิ่ม exact-prefix query.
8. เพิ่ม flow load-factor metric.
9. เพิ่ม rehash counter.
10. เพิ่ม iteration over live flows.

## Advanced
11. เพิ่ม trie pruning หลัง route remove.
12. ศึกษา Patricia/radix compression.
13. ขยายเป็น IPv6 128-bit prefixes.
14. เพิ่ม cuckoo/hash alternatives สำหรับ flow state.
15. วิเคราะห์ adversarial hash clustering.

## Implementation
16. เพิ่ม route export.
17. เพิ่ม batch flow lookup.
18. เพิ่ม allocation-failure tests.

## Challenge / Research
19. เปรียบเทียบ binary trie, Patricia trie และ TCAM conceptually.
20. ศึกษา RCU-based routing-table publication.
