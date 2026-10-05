# Theory — Lazy Propagation

## Representation equivalence

A deferred tag at an ancestor represents the same logical array as explicitly applying that delta to every descendant leaf.

Push changes representation but not abstract values.

## Correctness

Full-cover update:
aggregate formulas are exact for every element in the node interval.

Partial update:
push makes child aggregates current relative to the parent tag, recursion preserves child correctness, then pull reconstructs the parent.

## Query correctness

A const query accumulates every unpushed ancestor tag in carry.
Therefore fully covered node aggregate plus carry contribution equals the logical aggregate.
