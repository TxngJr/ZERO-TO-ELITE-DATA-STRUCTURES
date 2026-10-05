# Theory — Sparse Table

## Idempotence

An operation f is idempotent when:

    f(x,x)=x

For min:

    min(x,x)=x

This permits overlapping blocks in a query without double-counting changing the result.

## Query proof

Let block length B=2^floor(log2(r-l)).

Then B <= query length < 2B.

The left B-block and right B-block together cover the entire query, possibly with overlap.

Every query element appears in at least one chosen block.

Any overlap duplicates values, but min is idempotent, so the minimum over both blocks equals the minimum over the original range.

## Static trade-off

Sparse Table moves work from query time to preprocessing and memory.

This is useful when:
- dataset is fixed
- query count is large
- O(1) latency matters
