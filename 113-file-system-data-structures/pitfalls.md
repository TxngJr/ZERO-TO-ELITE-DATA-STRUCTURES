# Common Mistakes

- count indirect metadata block as file data.
- allocate indirect entries but forget the metadata block itself.
- grow partially then fail without rollback.
- shrink file but leak indirect metadata block.
- allow duplicate names under one directory.
- allow directory entry parent to be a file.
- let two inode pointers reference the same physical block accidentally.
- confuse inode number with physical block number.
