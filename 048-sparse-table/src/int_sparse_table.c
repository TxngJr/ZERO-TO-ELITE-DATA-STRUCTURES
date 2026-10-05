#include "int_sparse_table.h"

#include <stdint.h>
#include <stdlib.h>

struct IntSparseTable {
    size_t count;
    size_t levels;
    size_t *logs;
    int64_t *data;
};

static int64_t min_i64(int64_t a,int64_t b) {
    return a<b?a:b;
}

static size_t floor_log2_size(size_t value) {
    size_t result=0;

    while(value>1) {
        value>>=1;
        ++result;
    }

    return result;
}

IntSparseTable *int_sparse_table_create(
    const int64_t *values,
    size_t count
) {
    if(count>0&&values==NULL)return NULL;

    IntSparseTable *table=calloc(1,sizeof *table);
    if(table==NULL)return NULL;

    table->count=count;

    if(count==0)return table;

    if(count==SIZE_MAX ||
       count+1>SIZE_MAX/sizeof *table->logs) {
        free(table);
        return NULL;
    }

    table->levels=floor_log2_size(count)+1;

    if(table->levels>SIZE_MAX/count ||
       table->levels*count>
           SIZE_MAX/sizeof *table->data) {
        free(table);
        return NULL;
    }

    table->logs=calloc(
        count+1,
        sizeof *table->logs
    );
    table->data=calloc(
        table->levels*count,
        sizeof *table->data
    );

    if(table->logs==NULL||table->data==NULL) {
        int_sparse_table_free(table);
        return NULL;
    }

    for(size_t len=2;len<=count;++len) {
        table->logs[len]=table->logs[len/2]+1;
    }

    for(size_t i=0;i<count;++i) {
        table->data[i]=values[i];
    }

    for(size_t level=1;
        level<table->levels;
        ++level) {
        const size_t half=(size_t)1<<(level-1);
        const size_t length=half<<1;

        if(length>count)break;

        for(size_t i=0;i<=count-length;++i) {
            const int64_t a=
                table->data[(level-1)*count+i];
            const int64_t b=
                table->data[
                    (level-1)*count+i+half
                ];

            table->data[level*count+i]=
                min_i64(a,b);
        }
    }

    return table;
}

void int_sparse_table_free(IntSparseTable *table) {
    if(table==NULL)return;
    free(table->logs);
    free(table->data);
    free(table);
}

size_t int_sparse_table_size(
    const IntSparseTable *table
) {
    return table==NULL?0:table->count;
}

size_t int_sparse_table_levels(
    const IntSparseTable *table
) {
    return table==NULL?0:table->levels;
}

bool int_sparse_table_range_min(
    const IntSparseTable *table,
    size_t left,
    size_t right,
    int64_t *out_min
) {
    if(table==NULL||out_min==NULL||
       left>=right||right>table->count) {
        return false;
    }

    const size_t length=right-left;
    const size_t level=table->logs[length];
    const size_t block=(size_t)1<<level;

    const int64_t a=
        table->data[level*table->count+left];
    const int64_t b=
        table->data[
            level*table->count+right-block
        ];

    *out_min=min_i64(a,b);
    return true;
}

bool int_sparse_table_validate(
    const IntSparseTable *table
) {
    if(table==NULL)return false;

    if(table->count==0) {
        return table->levels==0 &&
               table->logs==NULL &&
               table->data==NULL;
    }

    if(table->levels!=
           floor_log2_size(table->count)+1 ||
       table->logs==NULL||
       table->data==NULL) {
        return false;
    }

    if(table->logs[1]!=0)return false;

    for(size_t len=2;len<=table->count;++len) {
        if(table->logs[len]!=
           table->logs[len/2]+1) {
            return false;
        }

        const size_t k=table->logs[len];

        if(((size_t)1<<k)>len)return false;

        if(k+1<table->levels &&
           ((size_t)1<<(k+1))<=len) {
            return false;
        }
    }

    for(size_t level=1;
        level<table->levels;
        ++level) {
        const size_t half=(size_t)1<<(level-1);
        const size_t length=half<<1;

        if(length>table->count)break;

        for(size_t i=0;
            i<=table->count-length;
            ++i) {
            const int64_t expected=min_i64(
                table->data[
                    (level-1)*table->count+i
                ],
                table->data[
                    (level-1)*table->count+i+half
                ]
            );

            if(table->data[
                    level*table->count+i
                ]!=expected) {
                return false;
            }
        }
    }

    return true;
}
