# Pitfalls — Persistent Segment Tree

- mutating a shared node
- cloning both subtrees unnecessarily
- storing raw pointers into a reallocating arena
- publishing root before update construction succeeds
- forgetting rollback of unpublished arena nodes after failure
- assuming persistence means automatic garbage collection
- confusing full persistence with latest-version-only updates
- copying entire arrays and calling it structural sharing
