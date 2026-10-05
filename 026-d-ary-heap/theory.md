# Theory — D-ary Heap

A complete d-ary tree stores roughly:

    1 + d + d^2 + ... + d^h

nodes through height h.

Thus height grows logarithmically base d.

For sift-down each visited node may compare up to d children.

Approximate comparison bound:

    O(d log_d n)

The function d/log d creates a real tuning trade-off rather than "larger d is always faster".

Bottom-up build remains linear because high-height nodes are exponentially rare.
