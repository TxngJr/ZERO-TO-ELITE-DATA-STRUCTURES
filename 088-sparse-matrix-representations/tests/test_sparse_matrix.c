#include "sparse_matrix.h"
#include <assert.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
static uint32_t rng(uint32_t*s){*s=*s*1664525u+1013904223u;return*s;}
int main(void){enum{R=37,C=29,OPS=5000};CooMatrix*coo=coo_matrix_create(R,C);assert(coo);double dense[R][C];memset(dense,0,sizeof dense);uint32_t s=0x88A12345u;for(int i=0;i<OPS;++i){size_t r=rng(&s)%R,c=rng(&s)%C;double v=(double)((int)(rng(&s)%11U)-5);if(v==0.0)continue;assert(coo_matrix_add(coo,r,c,v));dense[r][c]+=v;}assert(coo_matrix_validate(coo));CsrMatrix*csr=coo_matrix_to_csr(coo);CscMatrix*csc=coo_matrix_to_csc(coo);assert(csr&&csc);assert(csr_matrix_validate(csr)&&csc_matrix_validate(csc));assert(csr_matrix_nnz(csr)==csc_matrix_nnz(csc));for(size_t r=0;r<R;++r)for(size_t c=0;c<C;++c){double a=0,b=0;assert(csr_matrix_get(csr,r,c,&a));assert(csc_matrix_get(csc,r,c,&b));assert(fabs(a-dense[r][c])<1e-9);assert(fabs(b-dense[r][c])<1e-9);}double x[C],y[R],ref[R];for(size_t c=0;c<C;++c)x[c]=(double)(c%7)-3.0;for(size_t r=0;r<R;++r){ref[r]=0;for(size_t c=0;c<C;++c)ref[r]+=dense[r][c]*x[c];}assert(csr_matrix_spmv(csr,x,C,y,R));for(size_t r=0;r<R;++r)assert(fabs(y[r]-ref[r])<1e-9);csc_matrix_free(csc);csr_matrix_free(csr);coo_matrix_free(coo);puts("Sparse Matrix tests passed");return 0;}
