# Theory — Deque

## Why one head + size is enough

Given capacity C, head H, size N:

front position:
    H

back position:
    wrap(H+N-1)

next insertion at back:
    wrap(H+N)

No tail metadata required.

Fewer metadata fields mean fewer invariants, though some implementations choose head+tail for other reasons.

## Power-of-two optimization

A ring whose capacity is power of two can use:

    index & (capacity-1)

instead of modulo, but only if the power-of-two invariant is guaranteed.

The teaching implementation uses explicit wrap helpers to keep correctness independent of this optimization.

## Deque as a general worklist

Many algorithms need "take from one end, insert on either end". This is exactly the abstraction Deque provides.

Examples:
- 0-1 BFS
- monotonic candidate maintenance
- certain schedulers

## Monotonic candidate dominance

For window maximum, suppose old candidate j is behind current i and:

    value[i] >= value[j]

Since i expires later and is no smaller, j can never become maximum while i is in the same or a future overlapping window.

Therefore j is dominated and safe to remove from the back.
