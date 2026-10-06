#include "sparse_matrix.h"
#include <assert.h>
#include <stdio.h>
int main(void){CooMatrix*c=coo_matrix_create(3,3);assert(c);coo_matrix_add(c,0,0,2);coo_matrix_add(c,0,2,1);coo_matrix_add(c,2,1,4);CsrMatrix*r=coo_matrix_to_csr(c);assert(r);double x[]={1,2,3},y[3];assert(csr_matrix_spmv(r,x,3,y,3));printf("y=[%.1f %.1f %.1f] nnz=%zu\n",y[0],y[1],y[2],csr_matrix_nnz(r));csr_matrix_free(r);coo_matrix_free(c);return 0;}
