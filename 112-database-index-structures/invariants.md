# Invariants

1. primary rows sorted strictly by primary key.
2. primary keys are unique.
3. secondary entries sorted by (secondary_key, primary_key).
4. every secondary entry row_index is in range.
5. secondary entry primary/secondary keys match its referenced primary row.
6. every primary row appears exactly once in secondary index.
7. count is identical for both indexes.
