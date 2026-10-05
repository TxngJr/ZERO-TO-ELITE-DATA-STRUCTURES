# Implementation — LazySegmentTree

Arrays:
- sum[]
- min[]
- lazy[]

Range update mutates/pushes tags as needed.

Queries remain const by passing accumulated ancestor lazy values rather than normalizing the structure.

Validator checks the exact deferred-tag aggregate equations.
