# Implementation Notes

`AtomicBitset` allocates an array of `_Atomic uint64_t`. Public bit operations never touch padding outside `nbits`.

`AtomicTaggedValue` packs two uint32_t values into one uint64_t atomic. `atv_fetch_increment` uses weak CAS in a loop and returns retry count for contention measurement.

Runtime progress claims are guarded by `atomic_is_lock_free`; standard C does not promise every atomic type maps to lock-free hardware.
