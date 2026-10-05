# Invariants — AdaptiveGraph

1. backend is Adj List or Matrix only
2. backend vertex count equals wrapper vertex count
3. backend directedness semantics match wrapper
4. promote_percent > demote_percent
5. thresholds are within 0..100
6. backend validator passes
7. switch_count increments only after successful conversion
8. conversion preserves every logical edge and weight
