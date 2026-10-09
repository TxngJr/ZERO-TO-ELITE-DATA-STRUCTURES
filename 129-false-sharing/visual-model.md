# Visual Model

Packed:

```text
64-byte cache line
+----+----+----+----+----+----+----+----+
| c0 | c1 | c2 | c3 | c4 | c5 | c6 | c7 |
+----+----+----+----+----+----+----+----+
 T0   T1   T2   T3
```

Different atomic variables, same coherence line.

Padded:

```text
line 0: [c0 .........................]
line 1: [c1 .........................]
line 2: [c2 .........................]
line 3: [c3 .........................]
```

Each thread updates a distinct line.
