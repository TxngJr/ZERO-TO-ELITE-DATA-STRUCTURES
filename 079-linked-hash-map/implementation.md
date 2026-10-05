# Implementation

Heap-allocated stable nodes avoid index invalidation during rehash. Bucket chains are separate from order_prev/order_next links.
