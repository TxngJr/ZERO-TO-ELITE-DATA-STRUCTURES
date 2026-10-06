# Visual Model

```text
RAM directory:
[ first=1 last=253 ] [255..509] [511..765] ...

External blocks:
B0: 1,3,5,...253
B1: 255,257,...509
B2: ...

contains(401):
directory -> B1
1 logical block read
binary search inside B1
```
