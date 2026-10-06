# Invariants

1. `front == NULL` iff `front_len == 0`.
2. `rear == NULL` iff `rear_len == 0`.
3. list lengths ตรง metadata.
4. `front_len + rear_len` ไม่ overflow.
5. published nodes ไม่ถูก mutate.
6. logical order = `front ++ reverse(rear)`.
7. failed normalization ไม่ publish partial state.
