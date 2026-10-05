# Visual Model — Recursion & Iteration

## Call growth

fact(4)

    frame fact(4)
    +--------------------+
    | waits for fact(3)  |
    +--------------------+
             |
             v
    frame fact(3)
    +--------------------+
    | waits for fact(2)  |
    +--------------------+
             |
             v
    frame fact(2)
             |
             v
    frame fact(1)

Returns unwind upward.

## Iterative factorial

    i=2 result=1
      |
      v
    i=3 result=2
      |
      v
    i=4 result=6
      |
      v
    done result=24

Pending state stays in loop variables rather than recursive call chain.

## Binary search interval

    [0................16)
              mid
    choose one half

    [0........8)
        mid

    [4....8)
      ...

Each step discards roughly half the remaining interval.

## Recursive tree shape

          A
         / \
        B   C
       / \
      D   E

A recursive traversal mirrors this definition naturally, but maximum active call depth follows tree height, not node count directly.
