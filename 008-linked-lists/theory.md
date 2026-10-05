# Theory — Linked Lists

## Representation changes algorithmic capabilities

Array derives location from index arithmetic.
Linked list derives next location from data loaded from current node.

That is the essence of pointer chasing.

## Stable addresses

Growing an array may relocate all elements.
A separately allocated list node normally retains its address until that node is explicitly freed.

This can be useful for intrusive indexes, caches, schedulers and object graphs, but allocator behavior and ownership still matter.

## Known-position complexity

"Linked-list insertion is O(1)" assumes you already possess the insertion point/predecessor.

API:

    insert_at_index(list, i, x)

must locate index first → Θ(n)

API:

    insert_after(node, x)

can relink in Θ(1), ignoring allocation cost model detail.

Always state the operation contract.

## Intrusive vs non-intrusive preview

Non-intrusive:
list allocates wrapper Node containing payload/pointer.

Intrusive:
payload object itself contains next/prev links.

Intrusive structures:
- avoid wrapper allocation
- can support kernel/system patterns
- couple object to structure/lifetime rules

Chapter 137 will revisit deeply.

## Pool allocation preview

Per-node malloc can be expensive and scatter memory.
Memory pools/arenas can improve:
- allocation cost
- locality
- fragmentation behavior

Chapters 103–106 cover allocator structures.

## Cycle detection preview

A malformed singly list can accidentally form cycle.
Floyd's tortoise/hare algorithm detects cycle using O(1) auxiliary space.

The invariant checker in tests uses bounded counting/endpoint consistency; cycle detection is also demonstrated conceptually.
