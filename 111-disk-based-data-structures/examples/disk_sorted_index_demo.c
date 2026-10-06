#define _POSIX_C_SOURCE 200809L
#include "disk_sorted_index.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void){
    DiskRecord records[]={{10,100},{20,200},{30,300},{40,400}};
    char path[]="/tmp/zeds111-demo-XXXXXX";
    int fd=mkstemp(path);
    if(fd<0)return 1;
    close(fd);
    unlink(path);

    if(!dsi_create(path,records,4U))return 2;
    DiskSortedIndex*index=dsi_open(path);
    if(!index)return 3;

    bool found=false;
    int64_t value=0;
    if(!dsi_get(index,30U,&value,&found))return 4;
    printf("found=%d value=%lld seeks=%zu reads=%zu\n",
           found,(long long)value,dsi_logical_seeks(index),dsi_record_reads(index));

    dsi_close(index);
    unlink(path);
    return found?0:5;
}
