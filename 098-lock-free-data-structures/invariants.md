# Invariants

1. head is SIZE_MAX or a reserved slot index.
2. every slot is reserved at most once.
3. published node fields never change.
4. popped nodes are not reused before destroy.
5. following next from head reaches no cycle.
6. every reachable node index is below reserved count.
7. quiescent reachable count equals size metadata.
8. capacity is a lifetime push budget.
9. progress claim requires all atomics used by algorithm/instrumentation to report lock-free.
