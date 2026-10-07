# Theory — AI/ML Data Structures

ML systems spend significant time moving and indexing data, not only evaluating model math.

Common structures include:
- dense tensors
- sparse tensors
- datasets
- shuffled index permutations
- mini-batches
- embedding tables
- replay buffers
- feature stores
- vector indexes

This chapter focuses on contiguous dense storage + index indirection.

Shuffling indices instead of feature rows avoids copying the whole dataset each epoch. Batch gather then converts scattered logical row order into contiguous model input memory.

Embedding tables are lookup-oriented matrices: integer IDs select dense vectors.
