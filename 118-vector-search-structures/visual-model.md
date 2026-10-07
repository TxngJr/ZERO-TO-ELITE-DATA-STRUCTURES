# Visual Model

```text
vectors:
id0 [....]
id1 [....]
id2 [....]
...

query [....]
  |
  +-- distance(id0) --+
  +-- distance(id1) --+--> max-heap size k
  +-- distance(id2) --+        root = worst kept
                      |
                  sorted result
                  best -> worst
```

Exact baseline scans every stored vector.
