# Invariants — ByteCountingBloom

1. counter_count > 0
2. hash_count > 0
3. every hashed position is in range
4. counters never overflow or underflow
5. nonzero_count equals number of counters > 0
6. failed add due saturation rolls back its partial increments
7. failed remove due underflow rolls back its partial decrements
