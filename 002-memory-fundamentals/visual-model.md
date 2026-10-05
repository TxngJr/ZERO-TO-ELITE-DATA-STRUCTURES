# Visual Model — Memory Fundamentals

## Simplified process view

    higher virtual addresses
    +-------------------+
    | stack-like region |
    +-------------------+
    | mappings/libs     |
    +-------------------+
    | dynamic mappings  |
    | heap-related      |
    +-------------------+
    | data / bss        |
    +-------------------+
    | code              |
    +-------------------+
    lower virtual addresses

This is a mental model, not a guarantee of exact physical placement.

## Array locality

    address increases ->
    [a0][a1][a2][a3][a4]

Sequential traversal tends to reuse nearby fetched data.

## Pointer chasing

    [node A] -----> [node B] ----------> [node C]
       ^               ^                    ^
    unrelated addresses are possible

The next address may not be known until the current node is loaded.

## Ownership

    owner pointer ----> allocation
          |
          +-- responsible for eventual release

Borrowed aliases may access the allocation only while the ownership/lifetime contract permits.
