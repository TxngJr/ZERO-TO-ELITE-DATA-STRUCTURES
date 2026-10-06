# Visual Model

```text
Bounded Queue
+---+---+---+---+---+
| A | B |   |   |   |
+---+---+---+---+---+
  ^       ^
 head    tail

producer:
lock -> while(full) wait -> write -> size++ -> signal(not_empty) -> unlock

consumer:
lock -> while(empty && !closed) wait -> read -> size-- -> signal(not_full) -> unlock
```

Striped Map:

```text
bucket 0 [mutex] -> nodes
bucket 1 [mutex] -> nodes
...
bucket k [mutex] -> nodes
```
