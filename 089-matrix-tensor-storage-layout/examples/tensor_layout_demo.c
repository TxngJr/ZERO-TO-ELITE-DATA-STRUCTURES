#include "tensor_layout.h"
#include <assert.h>
#include <stdio.h>
int main(void){size_t shape[]={2,3};TensorLayout r,c;assert(tensor_layout_row_major(&r,shape,2));assert(tensor_layout_column_major(&c,shape,2));size_t i[]={1,2},a=0,b=0;assert(tensor_layout_offset(&r,i,&a));assert(tensor_layout_offset(&c,i,&b));printf("index[1,2] row-major=%zu column-major=%zu\n",a,b);return 0;}
