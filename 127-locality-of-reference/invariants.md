# Invariants

1. element_size > 0.
2. line_size > 0.
3. index × element_size never wraps.
4. unique_lines <= accesses.
5. first_line_touches == unique_lines.
6. temporal_line_reuses == accesses - unique_lines.
7. for accesses > 0, same_line_adjacent + line_transitions == accesses - 1.
8. adjacent_line_transitions <= line_transitions.
9. line ID 0 is a valid set key.
10. strided permutation visits every logical index exactly once.
