# Chapter 088 — Sparse Matrix Representations

Sparse matrices store only nonzero structure instead of every rows×cols cell.
This chapter implements COO as a mutable construction form and converts it into canonical CSR and CSC. Duplicate coordinates are sorted and summed; entries whose final sum is zero are removed.
CSR groups entries by row and supports efficient row traversal / sparse matrix-vector multiplication. CSC groups entries by column and favors column traversal.
