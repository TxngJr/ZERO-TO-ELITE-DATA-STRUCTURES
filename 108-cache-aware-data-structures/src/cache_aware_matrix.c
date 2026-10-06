#include "cache_aware_matrix.h"
#include <stdint.h>
#include <stdlib.h>

struct CacheAwareMatrix {
    size_t rows,cols;
    size_t tile_rows,tile_cols;
    size_t tile_row_count,tile_col_count;
    size_t tile_elems,total_tiles,total_elems;
    double *data;
};

static bool ceil_div(size_t n,size_t d,size_t*out){
    if(d==0U)return false;
    *out=n/d+(n%d!=0U);
    return true;
}

static bool mul_ok(size_t a,size_t b,size_t*out){
    if(a&&b>SIZE_MAX/a)return false;
    *out=a*b;
    return true;
}

CacheAwareMatrix*cam_create(size_t rows,size_t cols,size_t tr,size_t tc){
    if(rows==0U||cols==0U||tr==0U||tc==0U)return NULL;
    size_t trc=0U,tcc=0U,tile_elems=0U,total_tiles=0U,total_elems=0U;
    if(!ceil_div(rows,tr,&trc)||!ceil_div(cols,tc,&tcc)||
       !mul_ok(tr,tc,&tile_elems)||!mul_ok(trc,tcc,&total_tiles)||
       !mul_ok(total_tiles,tile_elems,&total_elems)||
       total_elems>SIZE_MAX/sizeof(double))return NULL;

    CacheAwareMatrix*m=calloc(1,sizeof(*m));
    if(!m)return NULL;
    m->data=calloc(total_elems,sizeof(*m->data));
    if(!m->data){free(m);return NULL;}

    m->rows=rows;m->cols=cols;
    m->tile_rows=tr;m->tile_cols=tc;
    m->tile_row_count=trc;m->tile_col_count=tcc;
    m->tile_elems=tile_elems;m->total_tiles=total_tiles;
    m->total_elems=total_elems;
    return m;
}

void cam_free(CacheAwareMatrix*m){if(!m)return;free(m->data);free(m);}
size_t cam_rows(const CacheAwareMatrix*m){return m?m->rows:0U;}
size_t cam_cols(const CacheAwareMatrix*m){return m?m->cols:0U;}
size_t cam_tile_rows(const CacheAwareMatrix*m){return m?m->tile_rows:0U;}
size_t cam_tile_cols(const CacheAwareMatrix*m){return m?m->tile_cols:0U;}

static bool logical_offset(const CacheAwareMatrix*m,size_t r,size_t c,size_t*out){
    if(!m||r>=m->rows||c>=m->cols)return false;
    size_t tr=r/m->tile_rows,tc=c/m->tile_cols;
    size_t ir=r%m->tile_rows,ic=c%m->tile_cols;
    size_t tile=tr*m->tile_col_count+tc;
    *out=tile*m->tile_elems+ir*m->tile_cols+ic;
    return true;
}

bool cam_set(CacheAwareMatrix*m,size_t r,size_t c,double value){
    size_t off=0U;
    if(!logical_offset(m,r,c,&off))return false;
    m->data[off]=value;
    return true;
}

bool cam_get(const CacheAwareMatrix*m,size_t r,size_t c,double*out){
    size_t off=0U;
    if(!out||!logical_offset(m,r,c,&off))return false;
    *out=m->data[off];
    return true;
}

bool cam_fill_from_dense(CacheAwareMatrix*m,const double*d,size_t count){
    if(!m||!d||m->rows>SIZE_MAX/m->cols||count<m->rows*m->cols)return false;
    for(size_t r=0;r<m->rows;++r)
        for(size_t c=0;c<m->cols;++c)
            if(!cam_set(m,r,c,d[r*m->cols+c]))return false;
    return true;
}

bool cam_copy_to_dense(const CacheAwareMatrix*m,double*d,size_t count){
    if(!m||!d||m->rows>SIZE_MAX/m->cols||count<m->rows*m->cols)return false;
    for(size_t r=0;r<m->rows;++r)
        for(size_t c=0;c<m->cols;++c){
            double v=0.0;
            if(!cam_get(m,r,c,&v))return false;
            d[r*m->cols+c]=v;
        }
    return true;
}

double cam_sum_row_order(const CacheAwareMatrix*m){
    if(!m)return 0.0;
    double sum=0.0;
    for(size_t r=0;r<m->rows;++r)
        for(size_t c=0;c<m->cols;++c){
            size_t off=0U;
            (void)logical_offset(m,r,c,&off);
            sum+=m->data[off];
        }
    return sum;
}

double cam_sum_tile_order(const CacheAwareMatrix*m){
    if(!m)return 0.0;
    double sum=0.0;
    for(size_t tr=0;tr<m->tile_row_count;++tr)
        for(size_t tc=0;tc<m->tile_col_count;++tc){
            size_t base=(tr*m->tile_col_count+tc)*m->tile_elems;
            for(size_t ir=0;ir<m->tile_rows;++ir){
                size_t r=tr*m->tile_rows+ir;
                if(r>=m->rows)break;
                for(size_t ic=0;ic<m->tile_cols;++ic){
                    size_t c=tc*m->tile_cols+ic;
                    if(c>=m->cols)break;
                    sum+=m->data[base+ir*m->tile_cols+ic];
                }
            }
        }
    return sum;
}

bool cam_transpose_to_dense(const CacheAwareMatrix*m,double*out,size_t count){
    if(!m||!out||m->rows>SIZE_MAX/m->cols||count<m->rows*m->cols)return false;
    for(size_t tr=0;tr<m->tile_row_count;++tr)
        for(size_t tc=0;tc<m->tile_col_count;++tc){
            size_t base=(tr*m->tile_col_count+tc)*m->tile_elems;
            for(size_t ir=0;ir<m->tile_rows;++ir){
                size_t r=tr*m->tile_rows+ir;
                if(r>=m->rows)break;
                for(size_t ic=0;ic<m->tile_cols;++ic){
                    size_t c=tc*m->tile_cols+ic;
                    if(c>=m->cols)break;
                    out[c*m->rows+r]=m->data[base+ir*m->tile_cols+ic];
                }
            }
        }
    return true;
}

bool cam_validate(const CacheAwareMatrix*m){
    if(!m||!m->data||m->rows==0U||m->cols==0U||
       m->tile_rows==0U||m->tile_cols==0U)return false;

    size_t trc=0U,tcc=0U,te=0U,tt=0U,total=0U;
    if(!ceil_div(m->rows,m->tile_rows,&trc)||
       !ceil_div(m->cols,m->tile_cols,&tcc)||
       !mul_ok(m->tile_rows,m->tile_cols,&te)||
       !mul_ok(trc,tcc,&tt)||!mul_ok(tt,te,&total))return false;

    if(trc!=m->tile_row_count||tcc!=m->tile_col_count||
       te!=m->tile_elems||tt!=m->total_tiles||
       total!=m->total_elems)return false;

    for(size_t tr=0;tr<m->tile_row_count;++tr)
        for(size_t tc=0;tc<m->tile_col_count;++tc){
            size_t base=(tr*m->tile_col_count+tc)*m->tile_elems;
            for(size_t ir=0;ir<m->tile_rows;++ir)
                for(size_t ic=0;ic<m->tile_cols;++ic){
                    size_t r=tr*m->tile_rows+ir;
                    size_t c=tc*m->tile_cols+ic;
                    if((r>=m->rows||c>=m->cols)&&
                       m->data[base+ir*m->tile_cols+ic]!=0.0)return false;
                }
        }
    return true;
}
