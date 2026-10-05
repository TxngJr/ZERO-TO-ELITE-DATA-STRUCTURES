# Visual Model — Deque

## Ring state

capacity=8, head=6, size=5

physical:
 0    1    2    3    4    5    6    7
+----+----+----+----+----+----+----+----+
| C  | D  | E  |    |    |    | A  | B  |
+----+----+----+----+----+----+----+----+
                              ^
                              front/head

logical:
A, B, C, D, E

push_front(X):
head moves from 6 to 5

push_back(Y):
writes at wrap(6+5)=3

## Monotonic queue for max

values in deque from front to back:

    [9,7,7,3]

non-increasing

front is current maximum candidate.
