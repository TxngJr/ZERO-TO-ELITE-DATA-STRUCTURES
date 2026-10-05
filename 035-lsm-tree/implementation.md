# Implementation — IntLSMTree

Entry:
- int key
- int value
- bool tombstone
- uint64_t sequence

MemTable:
- sorted unique-by-key entry array
- fixed configured capacity

Run:
- immutable sorted unique-by-key entries
- newest runs stored first

Tree:
- MemTable
- dynamic array of runs
- next sequence
- logical visible size

Mutations consult visible state first so logical size remains accurate.

Compaction builds replacement data before freeing old runs.
