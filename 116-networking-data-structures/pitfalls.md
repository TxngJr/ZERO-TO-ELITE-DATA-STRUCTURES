# Common Mistakes

- ทำ exact prefix match แทน longest-prefix match.
- ลืม default route ที่ root.
- ใช้ child raw pointers แล้ว realloc trie ทำ pointer dangling.
- shift IPv4 bits จาก LSB แทน network-prefix MSB.
- deletion ใน open addressing เปลี่ยน slot เป็น EMPTY แล้วทำ probe chain ขาด.
- hash struct ด้วย padding bytes ที่ไม่ deterministic.
- resize โดยทิ้ง tombstone semantics ไม่ถูกต้อง.
- อ้าง expected O(1) โดยไม่พูดถึง clustering/worst case.
