# Implementation

COO appends nonzero entries. Conversion copies/sorts entries, coalesces duplicate coordinates, removes zero sums, then builds prefix-pointer arrays. Canonical CSR/CSC keep indices strictly increasing inside each row/column.
