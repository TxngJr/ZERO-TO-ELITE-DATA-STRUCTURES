#include "tensor_layout.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
static double e(struct timespec a,struct timespec b){return(double)(b.tv_sec-a.tv_sec)+(double)(b.tv_nsec-a.tv_nsec)/1e9;}int main(void){const size_t rows=2048,cols=2048;double*x=malloc(rows*cols*sizeof*x);if(!x)return 1;for(size_t i=0;i<rows*cols;++i)x[i]=(double)(i&255U);volatile double sr=0,sc=0;struct timespec a,b,c,d;timespec_get(&a,TIME_UTC);for(size_t r=0;r<rows;++r)for(size_t col=0;col<cols;++col)sr+=x[r*cols+col];timespec_get(&b,TIME_UTC);timespec_get(&c,TIME_UTC);for(size_t col=0;col<cols;++col)for(size_t r=0;r<rows;++r)sc+=x[r*cols+col];timespec_get(&d,TIME_UTC);printf("row_scan=%.9f col_scan=%.9f checksum=%.0f/%.0f\n",e(a,b),e(c,d),(double)sr,(double)sc);free(x);return 0;}
