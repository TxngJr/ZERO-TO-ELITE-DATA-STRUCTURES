# Theory — Persistence and Structural Sharing

## Partial vs full persistence

Partial persistence:
- old versions can be queried
- updates only apply to newest version

Full persistence:
- any existing version can be used as the base for a new update

This chapter implements full persistence.

## Why O(log n) copied nodes?

A point belongs to exactly one child at each tree level.

Only ancestors of that point need changed aggregates.

The sibling at each level is identical between old and new versions, so it can be shared.

## Immutability proof idea

Published nodes are never mutated.

An update:
- allocates fresh nodes
- references old immutable subtrees
- publishes the new root only after successful construction

Therefore no update can change a path reachable from an older root.
