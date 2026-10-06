#ifndef SPARSE_MATRIX_H
#define SPARSE_MATRIX_H
#include <stdbool.h>
#include <stddef.h>
typedef struct CooMatrix CooMatrix;
typedef struct CsrMatrix CsrMatrix;
typedef struct CscMatrix CscMatrix;
CooMatrix *coo_matrix_create(size_t rows,size_t cols);
void coo_matrix_free(CooMatrix *m);
size_t coo_matrix_nnz(const CooMatrix *m);
bool coo_matrix_add(CooMatrix *m,size_t row,size_t col,double value);
bool coo_matrix_validate(const CooMatrix *m);
CsrMatrix *coo_matrix_to_csr(const CooMatrix *m);
CscMatrix *coo_matrix_to_csc(const CooMatrix *m);
void csr_matrix_free(CsrMatrix *m);
void csc_matrix_free(CscMatrix *m);
size_t csr_matrix_rows(const CsrMatrix *m);
size_t csr_matrix_cols(const CsrMatrix *m);
size_t csr_matrix_nnz(const CsrMatrix *m);
size_t csc_matrix_rows(const CscMatrix *m);
size_t csc_matrix_cols(const CscMatrix *m);
size_t csc_matrix_nnz(const CscMatrix *m);
bool csr_matrix_get(const CsrMatrix *m,size_t row,size_t col,double *out);
bool csc_matrix_get(const CscMatrix *m,size_t row,size_t col,double *out);
bool csr_matrix_spmv(const CsrMatrix *m,const double *x,size_t x_len,double *y,size_t y_len);
bool csr_matrix_validate(const CsrMatrix *m);
bool csc_matrix_validate(const CscMatrix *m);
#endif
