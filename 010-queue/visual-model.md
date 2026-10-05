# Visual Model — Queue

## Linked Queue

dequeue here                              enqueue here
    |                                         |
    v                                         v
 [10|*] -> [20|*] -> [30|NULL]
  head                       tail

## Circular Array

capacity=6, head=4, size=4

physical indexes:
  0    1    2    3    4    5
+----+----+----+----+----+----+
| 30 | 40 |    |    | 10 | 20 |
+----+----+----+----+----+----+
                      ^
                      head

logical sequence:
10 -> 20 -> 30 -> 40

next enqueue goes physical index 2.

## Grow

old logical:
10,20,30,40

new buffer:
+----+----+----+----+----+----+----+----+
| 10 | 20 | 30 | 40 |    |    |    |    |
+----+----+----+----+----+----+----+----+

head becomes 0.
