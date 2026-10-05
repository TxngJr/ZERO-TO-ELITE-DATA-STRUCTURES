# Pitfalls — Queue

- shifting array on every dequeue
- head/tail wrap off-by-one
- confusing full and empty when head==tail
- resizing wrapped buffer by raw memcpy
- forgetting head reset after grow
- linked dequeue leaving stale tail when last node removed
- using addition that can overflow before modulo
- assuming linked queue is thread-safe
- forgetting bounded-queue full policy
- queue benchmark comparing different semantics/workloads
