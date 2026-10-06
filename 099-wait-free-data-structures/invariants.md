# Invariants

1. capacity >= 2.
2. head,tail are valid physical indices.
3. one slot is always reserved.
4. producer is the only thread that writes tail.
5. consumer is the only thread that writes head.
6. producer writes payload before release-publishing tail.
7. consumer acquire-loads tail before reading payload.
8. consumer release-publishes head before producer reuses a slot.
9. progress claim requires head/tail atomics to be lock-free.
10. free occurs only after producer/consumer stop.
