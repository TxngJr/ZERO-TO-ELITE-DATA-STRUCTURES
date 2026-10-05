# Implementation — Set and Open-Addressing Map

## IntHashSet

Thin wrapper around IntIntHashTable:
- add -> put(value,1)
- contains -> contains(key)
- remove -> remove(key)
- size -> map size

This keeps Set API narrow.

## StringIntHashMap Slot

Each slot stores:
- char *key
- int value
- uint64_t hash
- SlotState state

## Capacity

Starts at 16 and doubles.

Capacity remains power-of-two because growth doubles, but implementation uses modulo for clarity and independence from low-bit assumptions.

## Insertion

1. hash borrowed key
2. search probe sequence
3. if existing key: update value
4. if new key: ensure load/tombstone policy
5. duplicate key string
6. occupy first tombstone or first empty slot
7. update size/tombstone counters

## Removal

1. find key
2. free owned key
3. clear key pointer
4. mark TOMBSTONE
5. size--
6. tombstones++

EMPTY would break probe continuity.

## Rehash

Allocate fresh slots first.
Move OCCUPIED entries without duplicating strings.
Commit only after allocation succeeds.
Tombstones reset to zero.
