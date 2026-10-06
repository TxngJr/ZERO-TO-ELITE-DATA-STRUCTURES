# Implementation

Shared state:
- atomic `head` index; `SIZE_MAX` = empty
- atomic `next_slot` lifetime allocator
- immutable node value/next after publication
- approximate atomic size
- CAS-failure counters

Slot reservation itself uses CAS and never lets `next_slot` exceed capacity. Publication CAS uses release semantics; pop acquire observes initialized node fields.

Pop uses CAS to move head to node.next. Nodes are not reused, so head cannot exhibit ABA from recycled node identity in this implementation.

`validate_quiescent` traverses only when workers have stopped and checks cycle/duplicate index plus size agreement.
