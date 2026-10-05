# Visual Model — Open Addressing

capacity=8

index: 0   1   2   3   4   5   6   7
      [ ] [A] [B] [T] [C] [ ] [ ] [ ]

Legend:
[ ] EMPTY
[T] TOMBSTONE
[A] OCCUPIED key A

Suppose home(D)=1:
1 occupied
2 occupied
3 tombstone -> remember
4 occupied
5 empty -> key absent

Insert D at remembered slot 3.

## Why lookup crosses tombstone

home(B)=1
B placed at 2 due collision.

If slot1 later deleted:
slot1 must be TOMBSTONE, not EMPTY,
or search for B would stop too early.

## Set over Map

IntHashSet
    |
    v
IntIntHashTable
key = set element
value = dummy 1
