#include "db_index.h"
#include <stdlib.h>

typedef struct {
    uint64_t secondary_key;
    uint64_t primary_key;
    size_t row_index;
} SecondaryEntry;

struct DbIndex {
    DbRow *primary;
    SecondaryEntry *secondary;
    size_t count;
};

static int cmp_primary(const void *a,const void *b){
    const DbRow *x=a,*y=b;
    if(x->primary_key<y->primary_key)return -1;
    if(x->primary_key>y->primary_key)return 1;
    return 0;
}

static int cmp_secondary(const void *a,const void *b){
    const SecondaryEntry *x=a,*y=b;
    if(x->secondary_key<y->secondary_key)return -1;
    if(x->secondary_key>y->secondary_key)return 1;
    if(x->primary_key<y->primary_key)return -1;
    if(x->primary_key>y->primary_key)return 1;
    return 0;
}

DbIndex *dbi_build(const DbRow *rows,size_t count){
    if(count>0U&&!rows)return NULL;
    if(count>SIZE_MAX/sizeof(DbRow)||
       count>SIZE_MAX/sizeof(SecondaryEntry))return NULL;

    DbIndex *index=calloc(1,sizeof(*index));
    if(!index)return NULL;
    index->count=count;
    if(count==0U)return index;

    index->primary=malloc(count*sizeof(*index->primary));
    index->secondary=malloc(count*sizeof(*index->secondary));
    if(!index->primary||!index->secondary){
        dbi_free(index);
        return NULL;
    }

    for(size_t i=0;i<count;++i)index->primary[i]=rows[i];
    qsort(index->primary,count,sizeof(*index->primary),cmp_primary);

    for(size_t i=1;i<count;++i){
        if(index->primary[i-1U].primary_key==
           index->primary[i].primary_key){
            dbi_free(index);
            return NULL;
        }
    }

    for(size_t i=0;i<count;++i){
        index->secondary[i]=(SecondaryEntry){
            index->primary[i].secondary_key,
            index->primary[i].primary_key,
            i
        };
    }
    qsort(index->secondary,count,sizeof(*index->secondary),cmp_secondary);
    return index;
}

void dbi_free(DbIndex *index){
    if(!index)return;
    free(index->primary);
    free(index->secondary);
    free(index);
}

size_t dbi_count(const DbIndex *index){return index?index->count:0U;}

static size_t primary_lower_bound(const DbIndex *index,uint64_t key){
    size_t lo=0U,hi=index->count;
    while(lo<hi){
        size_t mid=lo+(hi-lo)/2U;
        if(index->primary[mid].primary_key<key)lo=mid+1U;
        else hi=mid;
    }
    return lo;
}

bool dbi_get_primary(const DbIndex *index,uint64_t key,
                     DbRow *out_row,bool *out_found){
    if(!index||!out_row||!out_found)return false;
    size_t pos=primary_lower_bound(index,key);
    if(pos<index->count&&index->primary[pos].primary_key==key){
        *out_row=index->primary[pos];
        *out_found=true;
    }else{
        *out_found=false;
    }
    return true;
}

static size_t secondary_lower_bound(const DbIndex *index,uint64_t key){
    size_t lo=0U,hi=index->count;
    while(lo<hi){
        size_t mid=lo+(hi-lo)/2U;
        if(index->secondary[mid].secondary_key<key)lo=mid+1U;
        else hi=mid;
    }
    return lo;
}

bool dbi_secondary_range(const DbIndex *index,uint64_t low,uint64_t high,
                         DbRow *out,size_t out_capacity,size_t *out_count){
    if(!index||!out_count||low>high)return false;
    size_t pos=secondary_lower_bound(index,low);
    size_t produced=0U;

    while(pos<index->count&&index->secondary[pos].secondary_key<=high){
        if(produced>=out_capacity)return false;
        size_t row=index->secondary[pos].row_index;
        if(row>=index->count)return false;
        if(out)out[produced]=index->primary[row];
        ++produced;
        ++pos;
    }
    *out_count=produced;
    return true;
}

bool dbi_secondary_equal(const DbIndex *index,uint64_t key,
                         DbRow *out,size_t out_capacity,size_t *out_count){
    return dbi_secondary_range(index,key,key,out,out_capacity,out_count);
}

bool dbi_validate(const DbIndex *index){
    if(!index)return false;
    if(index->count==0U)return !index->primary&&!index->secondary;
    if(!index->primary||!index->secondary)return false;

    for(size_t i=1;i<index->count;++i)
        if(index->primary[i-1U].primary_key>=index->primary[i].primary_key)
            return false;

    unsigned char *seen=calloc(index->count,1U);
    if(!seen)return false;

    for(size_t i=0;i<index->count;++i){
        const SecondaryEntry *e=&index->secondary[i];
        if(e->row_index>=index->count||seen[e->row_index]){
            free(seen);
            return false;
        }
        seen[e->row_index]=1U;

        const DbRow *r=&index->primary[e->row_index];
        if(r->primary_key!=e->primary_key||
           r->secondary_key!=e->secondary_key){
            free(seen);
            return false;
        }

        if(i>0U){
            const SecondaryEntry *p=&index->secondary[i-1U];
            if(p->secondary_key>e->secondary_key||
               (p->secondary_key==e->secondary_key&&
                p->primary_key>=e->primary_key)){
                free(seen);
                return false;
            }
        }
    }

    free(seen);
    return true;
}
