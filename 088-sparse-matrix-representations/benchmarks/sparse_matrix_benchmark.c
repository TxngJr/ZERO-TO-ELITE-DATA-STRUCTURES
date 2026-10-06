#include "sparse_matrix.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
static double e(struct timespec a,struct timespec b){return(double)(b.tv_sec-a.tv_sec)+(double)(b.tv_nsec-a.tv_nsec)/1e9;}int main(void){const size_t n=5000,per=8;CooMatrix*c=coo_matrix_create(n,n);if(!c)return 1;for(size_t r=0;r<n;++r)for(size_t j=0;j<per;++j)if(!coo_matrix_add(c,r,(r*17+j*97)%n,1.0))return 1;struct timespec a,b;timespec_get(&a,TIME_UTC);CsrMatrix*csr=coo_matrix_to_csr(c);timespec_get(&b,TIME_UTC);if(!csr)return 1;double*x=malloc(n*sizeof*x),*y=malloc(n*sizeof*y);if(!x||!y)return 1;for(size_t i=0;i<n;++i)x[i]=1.0;struct timespec s,t;timespec_get(&s,TIME_UTC);for(int k=0;k<100;++k)if(!csr_matrix_spmv(csr,x,n,y,n))return 1;timespec_get(&t,TIME_UTC);printf("n=%zu nnz=%zu build=%.9f spmv100=%.9f\n",n,csr_matrix_nnz(csr),e(a,b),e(s,t));free(y);free(x);csr_matrix_free(csr);coo_matrix_free(c);return 0;}
