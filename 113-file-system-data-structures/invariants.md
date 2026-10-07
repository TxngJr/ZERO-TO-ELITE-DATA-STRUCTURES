# Invariants

1. inode 0 exists and is a directory.
2. every used non-root inode has exactly one directory entry.
3. every directory-entry parent is an allocated directory inode.
4. names are non-empty and unique within one parent.
5. file data_blocks = ceil(size_bytes / block_size).
6. first min(data_blocks,4) direct pointers are valid and remaining direct pointers are invalid.
7. indirect metadata block exists iff data_blocks > 4.
8. indirect entries exactly cover logical blocks after direct[0..3].
9. every referenced physical block is marked used.
10. no physical block is referenced more than once.
11. block bitmap has no extra marked blocks.
