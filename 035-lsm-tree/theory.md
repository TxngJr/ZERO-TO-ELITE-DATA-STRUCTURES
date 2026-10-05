# Theory — LSM Tree

## Immutable-run principle

Once a run is flushed, this model never edits it in place.

New versions live in newer structures.

Visibility is therefore determined by:
1. key equality
2. sequence/version order
3. tombstone semantics

## Write path

    Put/Delete
        |
        v
    MemTable
        |
      flush
        v
    immutable run
        |
    compaction
        v
    fewer/larger runs

## Read path

    MemTable -> newest run -> ... -> oldest run

Stop at first version of target key.

## Compaction correctness

For each key:
select maximum sequence version.

If it is live:
emit it.

If it is tombstone and all older immutable history participates:
emit nothing.

This preserves visible key/value state.
