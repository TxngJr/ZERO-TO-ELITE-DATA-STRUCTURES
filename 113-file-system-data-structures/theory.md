# Theory — File-System Data Structures

Filesystem ต้อง map namespace และ file offsets ไป physical storage.

Common structures:
- inode / file control block
- directory entries
- free-block bitmap
- free-inode bitmap
- direct/indirect block pointers
- extent trees
- allocation groups

บทนี้ใช้ direct + single-indirect mapping เพราะมองเห็น transition จาก small files ไป larger files ชัดเจน.

Production filesystems อาจใช้ extents, B-trees, journals, copy-on-write trees, checksums และ crash-consistency protocols.
