# Implementation — ByteCountingBloom

State:
- uint16_t counters[]
- counter_count
- hash_count
- logical_count
- cached nonzero_count

Add/remove are transactional with rollback for saturation/underflow discovered during the operation.
