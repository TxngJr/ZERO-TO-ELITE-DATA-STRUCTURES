# Visual Model — Linked Lists

## Singly

head                                      tail
 |                                          |
 v                                          v
+----+----+    +----+----+    +----+--------+
| 10 |  o---->| 20 |  o---->| 30 | NULL   |
+----+----+    +----+----+    +----+--------+

## Prepend 5

new
 |
 v
[5| ] ----+
          |
          v
        old head

then head=new

## Doubly

head                                         tail
 |                                             |
 v                                             v
+------+----+------+     +------+----+------+
| NULL | 10 | next |<--->| prev | 20 | NULL |
+------+----+------+     +------+----+------+

Every forward link must agree with matching backward link.

## Circular

tail->next == head

       +-----------------------+
       |                       |
       v                       |
      [A] -> [B] -> [C] -------+

Stop after size steps, not at NULL.
