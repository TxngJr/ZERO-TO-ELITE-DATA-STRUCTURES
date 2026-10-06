# Lab — Reachability and Cycles

1. Build 3,000-object graph.
2. Add random backward edges.
3. Add 120 roots.
4. Compute reachability independently.
5. Run `gc_collect`.
6. Compare every handle alive/dead against reference result.
7. Add an unreachable 2-node cycle and prove it is reclaimed.
8. Reuse one collected slot and prove old handle is stale.

Run ASan/UBSan + LeakSanitizer.
