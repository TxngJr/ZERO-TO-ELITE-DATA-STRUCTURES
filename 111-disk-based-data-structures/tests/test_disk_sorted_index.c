#define _POSIX_C_SOURCE 200809L
#include "disk_sorted_index.h"
#include <assert.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

enum{N=50000,QUERIES=20000};
static uint64_t state=UINT64_C(0x8877665544332211);
static uint64_t rng(void){state^=state<<13;state^=state>>7;state^=state<<17;return state;}

int main(void){
    DiskRecord*records=malloc((size_t)N*sizeof(*records));
    assert(records);
    for(size_t i=0;i<N;++i){
        records[i].key=(uint64_t)i*3U+2U;
        records[i].value=(int64_t)i*11-7;
    }

    char path[]="/tmp/zeds111-index-XXXXXX";
    int fd=mkstemp(path);
    assert(fd>=0);
    close(fd);
    assert(unlink(path)==0);

    assert(dsi_create(path,records,N));
    DiskSortedIndex*index=dsi_open(path);
    assert(index&&dsi_count(index)==N);
    assert(dsi_validate(index));

    size_t seeks_before=dsi_logical_seeks(index);
    size_t reads_before=dsi_record_reads(index);
    dsi_reset_io_counters(index);
    assert(dsi_logical_seeks(index)==0U&&dsi_record_reads(index)==0U);

    for(size_t q=0;q<QUERIES;++q){
        uint64_t key=rng()%((uint64_t)N*3U+100U);
        bool found=false;
        int64_t value=0;
        assert(dsi_get(index,key,&value,&found));
        bool expected=key>=2U&&key<(uint64_t)N*3U+2U&&((key-2U)%3U==0U);
        assert(found==expected);
        if(found){
            size_t i=(size_t)((key-2U)/3U);
            assert(value==(int64_t)i*11-7);
        }
    }
    assert(dsi_logical_seeks(index)>0U&&dsi_record_reads(index)>0U);

    DiskRecord range[256];
    size_t range_count=0U;
    dsi_reset_io_counters(index);
    assert(dsi_range_collect(index,3002U,3602U,range,256U,&range_count));
    assert(range_count==201U);
    for(size_t i=0;i<range_count;++i){
        size_t idx=1000U+i;
        assert(range[i].key==(uint64_t)idx*3U+2U);
        assert(range[i].value==(int64_t)idx*11-7);
    }
    assert(dsi_logical_seeks(index)>0U);
    assert(dsi_record_reads(index)>=range_count);

    assert(dsi_validate(index));
    assert(dsi_logical_seeks(index)>0U);
    (void)seeks_before;(void)reads_before;
    dsi_close(index);

    DiskSortedIndex*reopened=dsi_open(path);
    assert(reopened&&dsi_validate(reopened));
    dsi_close(reopened);
    assert(unlink(path)==0);

    char bad[]="/tmp/zeds111-bad-XXXXXX";
    fd=mkstemp(bad);
    assert(fd>=0);
    const char garbage[]="bad";
    assert(write(fd,garbage,sizeof(garbage))==(ssize_t)sizeof(garbage));
    close(fd);
    assert(dsi_open(bad)==NULL);
    assert(unlink(bad)==0);

    puts("Disk-sorted index tests passed");
    free(records);
    return 0;
}
