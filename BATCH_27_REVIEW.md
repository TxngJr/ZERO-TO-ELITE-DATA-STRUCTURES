# Batch 27 — Negative Review and Audit

## Scope

- 079 Linked Hash Map
- 080 Ordered Map / Ordered Set
- 081 Multiset / Multimap

## Chapter 079 Review

Checked:
- bucket chains and order list use independent pointers
- every live node appears once in insertion-order list
- every live node appears once in its hashed bucket chain
- overwrite updates value without changing order
- rehash rewires only bucket links and leaves order links intact
- removal unlinks from both bucket chain and doubly-linked order list
- head/tail transitions are handled for first/last/only nodes
- validator counts both structures and checks equality with size

Complexity:
- expected lookup/update/remove O(1)
- iteration O(n)
- rehash O(n)

## Chapter 080 Review

Checked:
- Map and Set use strict ascending key order
- lower_bound implementation is half-open binary search
- duplicate Set add is idempotent
- duplicate Map put overwrites value without changing size
- insert/remove memmove lengths avoid underflow when operating at end
- size/capacity/storage validator checks are explicit
- documentation distinguishes sorted order from insertion order

Complexity:
- lookup/lower_bound O(log n)
- nth O(1)
- insertion/removal O(n)
- storage O(n)

## Chapter 081 Review

Checked:
- Multiset stores strictly increasing distinct keys
- each stored multiplicity is positive
- total cardinality equals sum of counts
- zero-count add is no-op
- over-removal fails without mutation
- removing final occurrence erases distinct entry
- Multimap keys are nondecreasing
- lower_bound/upper_bound make equal-key values contiguous
- insertion at upper_bound preserves insertion order among equal keys
- remove-one removes one matching key/value occurrence
- remove-all compacts the remaining suffix and returns exact removed count

Complexity:
- lookup/count O(log n) plus equal-range work
- array insertion/removal O(n)
- Multiset compressed storage O(distinct)
- Multimap storage O(pairs)

## Testing Review

- Linked Hash Map: rehash pressure, overwrite-order preservation, alternating removals
- Ordered Map/Set: descending insertion stress and sorted iteration
- Multiset/Multimap: multiplicity, stable equal-key ordering and erase semantics
- CI retains warnings, ASan/UBSan and 60-second per-test watchdog

## Definition of Done

Batch 27 is complete only when Fedora CI configures/builds Chapters 001–081 and the full CTest suite passes with sanitizers enabled.
