# Complexity

COO append amortized O(1). Conversion O(k log k) due sorting. CSR get O(log nnz_row), CSC get O(log nnz_col). CSR SpMV O(rows+nnz). Storage O(rows+nnz) or O(cols+nnz).
