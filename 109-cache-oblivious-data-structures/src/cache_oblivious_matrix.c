#include "cache_oblivious_matrix.h"
#include <stdint.h>
#include <stdlib.h>

struct CacheObliviousMatrix{
    size_t rows,cols,side,total;
    double *data;
};

static bool next_pow2(size_t x,size_t*out){
    if(x==0U)return false;
    size_t p=1U;
    while(p<x){
        if(p>SIZE_MAX/2U)return false;
        p*=2U;
    }
    *out=p;
    return true;
}

static bool morton_index(size_t row,size_t col,size_t side,size_t*out){
    size_t result=0U;
    size_t bit=0U;
    for(size_t mask=1U;mask<side;mask<<=1U,++bit){
        if(bit>=sizeof(size_t)*4U)return false;
        size_t shift=bit*2U;
        if(col&mask)result|=(size_t)1U<<shift;
        if(row&mask)result|=(size_t)1U<<(shift+1U);
    }
    *out=result;
    return true;
}

CacheObliviousMatrix*com_create(size_t rows,size_t cols){
    if(rows==0U||cols==0U)return NULL;
    size_t maxdim=rows>cols?rows:cols,side=0U;
    if(!next_pow2(maxdim,&side)||side>SIZE_MAX/side)return NULL;
    size_t total=side*side;
    if(total>SIZE_MAX/sizeof(double))return NULL;
    CacheObliviousMatrix*m=calloc(1,sizeof(*m));
    if(!m)return NULL;
    m->data=calloc(total,sizeof(*m->data));
    if(!m->data){free(m);return NULL;}
    m->rows=rows;m->cols=cols;m->side=side;m->total=total;
    return m;
}

void com_free(CacheObliviousMatrix*m){if(!m)return;free(m->data);free(m);}
size_t com_rows(const CacheObliviousMatrix*m){return m?m->rows:0U;}
size_t com_cols(const CacheObliviousMatrix*m){return m?m->cols:0U;}
size_t com_padded_side(const CacheObliviousMatrix*m){return m?m->side:0U;}

bool com_set(CacheObliviousMatrix*m,size_t r,size_t c,double value){
    if(!m||r>=m->rows||c>=m->cols)return false;
    size_t idx=0U;
    if(!morton_index(r,c,m->side,&idx)||idx>=m->total)return false;
    m->data[idx]=value;
    return true;
}

bool com_get(const CacheObliviousMatrix*m,size_t r,size_t c,double*out){
    if(!m||!out||r>=m->rows||c>=m->cols)return false;
    size_t idx=0U;
    if(!morton_index(r,c,m->side,&idx)||idx>=m->total)return false;
    *out=m->data[idx];
    return true;
}

bool com_copy_to_dense(const CacheObliviousMatrix*m,double*out,size_t count){
    if(!m||!out||m->rows>SIZE_MAX/m->cols||count<m->rows*m->cols)return false;
    for(size_t r=0;r<m->rows;++r)
        for(size_t c=0;c<m->cols;++c){
            double v=0.0;
            if(!com_get(m,r,c,&v))return false;
            out[r*m->cols+c]=v;
        }
    return true;
}

double com_sum_row_order(const CacheObliviousMatrix*m){
    if(!m)return 0.0;
    double sum=0.0;
    for(size_t r=0;r<m->rows;++r)
        for(size_t c=0;c<m->cols;++c){
            size_t idx=0U;
            (void)morton_index(r,c,m->side,&idx);
            sum+=m->data[idx];
        }
    return sum;
}

double com_sum_z_order(const CacheObliviousMatrix*m){
    if(!m)return 0.0;
    double sum=0.0;
    for(size_t i=0;i<m->total;++i)sum+=m->data[i];
    return sum;
}

bool com_transpose_to_dense(const CacheObliviousMatrix*m,double*out,size_t count){
    if(!m||!out||m->rows>SIZE_MAX/m->cols||count<m->rows*m->cols)return false;
    for(size_t r=0;r<m->rows;++r)
        for(size_t c=0;c<m->cols;++c){
            double v=0.0;
            if(!com_get(m,r,c,&v))return false;
            out[c*m->rows+r]=v;
        }
    return true;
}

bool com_validate(const CacheObliviousMatrix*m){
    if(!m||!m->data||m->rows==0U||m->cols==0U||m->side==0U)return false;
    if((m->side&(m->side-1U))!=0U||m->side<m->rows||m->side<m->cols||
       m->side>SIZE_MAX/m->side||m->total!=m->side*m->side)return false;

    for(size_t r=0;r<m->side;++r)
        for(size_t c=0;c<m->side;++c){
            if(r<m->rows&&c<m->cols)continue;
            size_t idx=0U;
            if(!morton_index(r,c,m->side,&idx)||idx>=m->total)return false;
            if(m->data[idx]!=0.0)return false;
        }
    return true;
}
