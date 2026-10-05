# Pitfalls — Lazy Propagation

- updating parent aggregate but forgetting lazy tag
- pushing after recursion instead of before partial descent
- forgetting interval length in sum update
- composing tags in wrong order
- pulling while parent still owns an unapplied lazy tag
- mutating tree inside a logically const query unnecessarily
- mixing [l,r] with [l,r)
- claiming every lazy operation is safe from integer overflow
