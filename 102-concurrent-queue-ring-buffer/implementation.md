# Implementation Notes

Capacity is fixed and power-of-two. Every cell owns one atomic sequence.

The code avoids signed-difference wrap tricks by imposing a practical no-counter-wrap lifetime contract. Near the overflow boundary operations return API failure.

`cmr_size_quiescent` is explicitly quiescent-only. During in-flight reservation, enqueue/dequeue positions can move before cell publication/retirement completes, so interpreting their difference as exact live size concurrently would be misleading.
