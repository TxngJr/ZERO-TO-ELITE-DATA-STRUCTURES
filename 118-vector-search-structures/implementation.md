# Implementation Notes

Vectors are append-only and stored row-major in one contiguous float array.

Each add computes and stores squared norm. Non-finite or zero-norm vectors are rejected so both L2 and cosine metrics remain well-defined.

Search uses a max-heap of `VectorSearchResult`. “Worse” means larger distance; equal distance uses larger ID as worse. Final heap contents are sorted ascending by distance then ID.

Capacity grows geometrically with overflow checks before reallocating vector and norm arrays.
