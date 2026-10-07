# Visual Model

```text
Interner:
hash("x") -> NameId 4 -> "x"

Bindings:
current[4] -> symbol #9 depth=2
                 |
                 v
             symbol #5 depth=1
                 |
                 v
             symbol #1 depth=0

leave depth=2:
current[4] = symbol #5
```

Def-use:

```text
head[3] -> edge(user=8) -> edge(user=11) -> END
```
