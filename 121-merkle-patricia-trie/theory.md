# Theory — Merkle Patricia Trie

Patricia trie บีบ chain ที่มี child เดียวให้เป็น path fragment. Merkle authentication เพิ่ม hash commitment ให้ทุก node.

ผลคือ:
- prefix navigation จาก trie
- fewer structural nodes จาก path compression
- deterministic root commitment ต่อ key/value set

บทนี้ใช้ fixed-width 256-bit keys เพื่อหลีกเลี่ยง terminal-value ambiguity ที่ branch nodes. ระบบจริงอาจใช้ variable-length keys และ richer node serialization เช่น RLP/SSZ/custom codecs.

Root correctness ต้องขึ้นกับ canonical node encoding ไม่ใช่ pointer address หรือ insertion order.
