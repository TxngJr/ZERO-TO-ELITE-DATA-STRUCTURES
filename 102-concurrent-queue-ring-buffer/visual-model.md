# Visual Model

```text
capacity = 4

logical position 5
physical slot = 5 & 3 = 1

cell[1].sequence distinguishes:
  1   -> old generation
  5   -> producer-ready for position 5
  6   -> published item for consumer position 5
  9   -> reusable after dequeue generation
```

Producer:

```text
CAS enqueue_pos
write data
release sequence
```

Consumer:

```text
acquire sequence
CAS dequeue_pos
read data
release next-generation sequence
```
