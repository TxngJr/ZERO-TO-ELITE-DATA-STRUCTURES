# Implementation Notes

`MiniFs` preallocates:
- inode array
- inode-used bitmap
- physical-block bitmap
- directory-entry array

Root inode is inode 0 and always a directory.

File resize uses logical block count `ceil(size/block_size)`. Growth preselects all blocks before commit. Shrink releases highest logical data blocks first and releases the indirect metadata block when the file returns to <=4 data blocks.

Directory lookup is linear in this teaching model; Chapter 113 focuses metadata relationships rather than implementing a directory hash/B-tree.
