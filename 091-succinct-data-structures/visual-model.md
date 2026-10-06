# Visual Model

```text
logical bits
0...............................................................63 64...
|---------------------- word 0 -------------------------------| |word1|

packed:
words[0]  words[1] ... words[7] | words[8] ...
\________ superblock 0 ________/ \__ superblock 1

super_rank:
[0] = ones before superblock 0 = 0
[1] = ones before superblock 1
[2] = ones before superblock 2
...
```

Rank query at position `p`:

```text
checkpoint
   |
   v
[known prefix] + popcount(whole local words) + popcount(masked tail)
```

Select query:

```text
k-th 1
  |
binary search super_rank
  |
scan <= 8 words
  |
scan bits in one word
  v
physical bit index
```
