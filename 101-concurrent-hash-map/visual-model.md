# Visual Model

Normal operation:

```text
table RW lock: READ
      |
      +--> bucket[hash(key)] mutex
                 |
                 +--> chain
```

Resize:

```text
WRITE lock
old[8 buckets] --rehash--> new[16 buckets]
publish new pointer/count
destroy old bucket mutexes
unlock
```

Write lock excludes every operation that could still hold an old bucket pointer.
