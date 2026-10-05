# Theory — Consistent Hashing

Traditional modulo hashing can remap most keys when node count changes.
Consistent hashing changes ownership only around inserted/removed ring intervals.
Virtual nodes approximate smoother ownership across heterogeneous hash intervals.
