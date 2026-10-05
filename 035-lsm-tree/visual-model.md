# Visual Model — Mini LSM

MemTable:
    5 -> 500
    9 -> TOMBSTONE

Runs newest first:

Run 0:
    1 -> 100
    9 -> TOMBSTONE
   20 -> 200

Run 1:
    2 -> 20
    9 -> 90
   30 -> 300

Lookup 9:
- MemTable tombstone wins immediately
- older 9 -> 90 is hidden

After full compaction of immutable runs:
Run:
    1 -> 100
    2 -> 20
   20 -> 200
   30 -> 300

9 disappears because newest immutable version was tombstone and no older immutable history remains.
