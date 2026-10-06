# Theory — Sparse Matrices

COO stores triplets (row,col,value). CSR stores row_ptr, col_idx, values. CSC stores col_ptr, row_idx, values.
Representation should follow access pattern: row algorithms favor CSR, column algorithms favor CSC. Sparse storage only wins when metadata cost is lower than dense zeros avoided.
