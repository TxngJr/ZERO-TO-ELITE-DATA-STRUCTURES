#define _POSIX_C_SOURCE 200809L
#include "disk_sorted_index.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

enum{N=200000,QUERIES=100000};
static double elapsed(struct timespec a,struct timespec b){
    return(double)(b.tv_sec-a.tv_sec)+(double)(b.tv_nsec-a.tv_nsec)/1e9;
}

int main(void){
    DiskRecord*records=malloc((size_t)N*sizeof(*records));
    if(!records)return 1;
    for(size_t i=0;i<N;++i)records[i]=(DiskRecord){(uint64_t)i*2U+1U,(int64_t)i};

    char path[]="/tmp/zeds111-bench-XXXXXX";
    int fd=mkstemp(path);
    if(fd<0)return 2;
    close(fd);unlink(path);
    if(!dsi_create(path,records,N))return 3;

    DiskSortedIndex*index=dsi_open(path);
    if(!index)return 4;
    dsi_reset_io_counters(index);

    struct timespec a,b;
    timespec_get(&a,TIME_UTC);
    size_t hits=0U;
    for(size_t q=0;q<QUERIES;++q){
        uint64_t key=(uint64_t)(q*7919U)%((uint64_t)N*2U);
        bool found=false;int64_t value=0;
        if(!dsi_get(index,key,&value,&found))return 5;
        hits+=found?1U:0U;
    }
    timespec_get(&b,TIME_UTC);

    printf("records=%d queries=%d seconds=%.6f seeks=%zu reads=%zu hits=%zu\n",
           N,QUERIES,elapsed(a,b),dsi_logical_seeks(index),
           dsi_record_reads(index),hits);

    dsi_close(index);unlink(path);free(records);return 0;
}
