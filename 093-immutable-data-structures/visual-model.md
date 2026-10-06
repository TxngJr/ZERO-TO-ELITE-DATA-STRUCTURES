# Visual Model

```text
input: [3, 1, 3, 7, 9]
             |
             v
      build / deduplicate
             |
             v
capacity = 16
slot:  0 1 2 3 4 5 6 7 ...
used:  . X . . X X . . ...
keys:    9     1 3      ...
             ^
             |
        frozen after build
```

Reader path:

```text
hash(key)
   |
mask capacity-1
   |
linear probe
   |
found / first empty => not found
```

ไม่มี path สำหรับ `remove`, `insert`, `rehash` หลัง publish.
