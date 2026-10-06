# Invariants

1. source values strictly increase
2. block_size>0
3. block_count=ceil(n/block_size)
4. offsets are monotonic and end at byte_count
5. each decoded gap is positive
6. decoded sequence remains strictly increasing without uint64 overflow
