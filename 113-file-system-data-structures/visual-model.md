# Visual Model

```text
directory entry:
(root,"docs") -> inode 1
(1,"a.txt")   -> inode 2

inode 2, size = 7 blocks:
direct[0] -> block 10
direct[1] -> block 11
direct[2] -> block 12
direct[3] -> block 13
indirect metadata -> block 14
  [0] -> block 15
  [1] -> block 16
  [2] -> block 17
```

Block bitmap marks blocks 10..17 used, including indirect metadata.
