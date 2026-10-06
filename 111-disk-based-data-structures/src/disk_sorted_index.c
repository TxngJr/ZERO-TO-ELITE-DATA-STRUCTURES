#define _POSIX_C_SOURCE 200809L
#include "disk_sorted_index.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

enum { HEADER_SIZE=64, RECORD_SIZE=16 };
static const unsigned char MAGIC[8]={'Z','D','S','I','D','X','1','1'};

struct DiskSortedIndex{
    FILE *fp;
    size_t count;
    size_t logical_seeks;
    size_t record_reads;
};

static void put_u64_le(unsigned char*p,uint64_t v){
    for(unsigned i=0;i<8U;++i)p[i]=(unsigned char)(v>>(i*8U));
}

static uint64_t get_u64_le(const unsigned char*p){
    uint64_t v=0U;
    for(unsigned i=0;i<8U;++i)v|=(uint64_t)p[i]<<(i*8U);
    return v;
}

static uint64_t i64_to_twos(int64_t v){
    if(v>=0)return (uint64_t)v;
    uint64_t magnitude=(uint64_t)(-(v+1));
    return UINT64_MAX-magnitude;
}

static int64_t twos_to_i64(uint64_t bits){
    if(bits<=(uint64_t)INT64_MAX)return (int64_t)bits;
    uint64_t magnitude=UINT64_MAX-bits;
    return -(int64_t)magnitude-1;
}

static void put_i64_le(unsigned char*p,int64_t v){
    put_u64_le(p,i64_to_twos(v));
}

static int64_t get_i64_le(const unsigned char*p){
    return twos_to_i64(get_u64_le(p));
}

static bool record_offset(size_t index,off_t*out){
    const uint64_t max_off=(uint64_t)INT64_MAX;
    if((uint64_t)index>(max_off-HEADER_SIZE)/RECORD_SIZE)return false;
    *out=(off_t)(HEADER_SIZE+(uint64_t)index*RECORD_SIZE);
    return true;
}

static bool write_record(FILE*fp,DiskRecord r){
    unsigned char buf[RECORD_SIZE];
    put_u64_le(buf,r.key);
    put_i64_le(buf+8,r.value);
    return fwrite(buf,1U,sizeof(buf),fp)==sizeof(buf);
}

bool dsi_create(const char*path,const DiskRecord*records,size_t count){
    if(!path||(count>0U&&!records))return false;
    for(size_t i=1;i<count;++i)if(records[i-1U].key>=records[i].key)return false;

    off_t checked_offset=0;
    if(count>0U&&!record_offset(count-1U,&checked_offset))return false;

    FILE*fp=fopen(path,"wb");
    if(!fp)return false;
    unsigned char header[HEADER_SIZE]={0};
    memcpy(header,MAGIC,sizeof(MAGIC));
    put_u64_le(header+8,1U);
    put_u64_le(header+16,(uint64_t)count);
    put_u64_le(header+24,RECORD_SIZE);

    bool ok=fwrite(header,1U,sizeof(header),fp)==sizeof(header);
    for(size_t i=0;ok&&i<count;++i)ok=write_record(fp,records[i]);
    if(ok)ok=fflush(fp)==0;
    if(fclose(fp)!=0)ok=false;
    if(!ok)remove(path);
    return ok;
}

static bool read_header(FILE*fp,size_t*out_count){
    unsigned char header[HEADER_SIZE];
    if(fseeko(fp,0,SEEK_SET)!=0)return false;
    if(fread(header,1U,sizeof(header),fp)!=sizeof(header))return false;
    if(memcmp(header,MAGIC,sizeof(MAGIC))!=0)return false;
    if(get_u64_le(header+8)!=1U||get_u64_le(header+24)!=RECORD_SIZE)return false;
    for(size_t i=32U;i<HEADER_SIZE;++i)if(header[i]!=0U)return false;

    uint64_t encoded_count=get_u64_le(header+16);
    size_t count=(size_t)encoded_count;
    if((uint64_t)count!=encoded_count)return false;
    *out_count=count;
    return true;
}

DiskSortedIndex*dsi_open(const char*path){
    if(!path)return NULL;
    FILE*fp=fopen(path,"rb");
    if(!fp)return NULL;
    size_t count=0U;
    if(!read_header(fp,&count)){fclose(fp);return NULL;}

    off_t expected=0;
    if(count==0U)expected=HEADER_SIZE;
    else{
        off_t last=0;
        if(!record_offset(count-1U,&last)){fclose(fp);return NULL;}
        expected=last+RECORD_SIZE;
    }
    if(fseeko(fp,0,SEEK_END)!=0){fclose(fp);return NULL;}
    off_t actual=ftello(fp);
    if(actual<0||actual!=expected){fclose(fp);return NULL;}

    DiskSortedIndex*index=calloc(1,sizeof(*index));
    if(!index){fclose(fp);return NULL;}
    index->fp=fp;
    index->count=count;
    return index;
}

void dsi_close(DiskSortedIndex*index){
    if(!index)return;
    if(index->fp)fclose(index->fp);
    free(index);
}
size_t dsi_count(const DiskSortedIndex*index){return index?index->count:0U;}
size_t dsi_logical_seeks(const DiskSortedIndex*index){return index?index->logical_seeks:0U;}
size_t dsi_record_reads(const DiskSortedIndex*index){return index?index->record_reads:0U;}
void dsi_reset_io_counters(DiskSortedIndex*index){
    if(index){index->logical_seeks=0U;index->record_reads=0U;}
}

static bool read_record_at(DiskSortedIndex*index,size_t pos,DiskRecord*out,bool count_io){
    if(!index||!out||pos>=index->count)return false;
    off_t off=0;
    if(!record_offset(pos,&off))return false;
    if(fseeko(index->fp,off,SEEK_SET)!=0)return false;
    unsigned char buf[RECORD_SIZE];
    if(fread(buf,1U,sizeof(buf),index->fp)!=sizeof(buf))return false;
    if(count_io){++index->logical_seeks;++index->record_reads;}
    out->key=get_u64_le(buf);
    out->value=get_i64_le(buf+8);
    return true;
}

static bool lower_bound_index(DiskSortedIndex*index,uint64_t key,size_t*out_pos){
    size_t lo=0U,hi=index->count;
    while(lo<hi){
        size_t mid=lo+(hi-lo)/2U;
        DiskRecord r;
        if(!read_record_at(index,mid,&r,true))return false;
        if(r.key<key)lo=mid+1U;else hi=mid;
    }
    *out_pos=lo;
    return true;
}

bool dsi_get(DiskSortedIndex*index,uint64_t key,int64_t*out_value,bool*out_found){
    if(!index||!out_value||!out_found)return false;
    size_t pos=0U;
    if(!lower_bound_index(index,key,&pos))return false;
    if(pos==index->count){*out_found=false;return true;}
    DiskRecord r;
    if(!read_record_at(index,pos,&r,true))return false;
    *out_found=r.key==key;
    if(*out_found)*out_value=r.value;
    return true;
}

bool dsi_range_collect(DiskSortedIndex*index,uint64_t low,uint64_t high,
                       DiskRecord*out,size_t out_capacity,size_t*out_count){
    if(!index||!out_count||low>high)return false;
    size_t pos=0U;
    if(!lower_bound_index(index,low,&pos))return false;
    if(pos==index->count){*out_count=0U;return true;}

    off_t off=0;
    if(!record_offset(pos,&off)||fseeko(index->fp,off,SEEK_SET)!=0)return false;
    ++index->logical_seeks;

    size_t produced=0U;
    for(size_t i=pos;i<index->count;++i){
        unsigned char buf[RECORD_SIZE];
        if(fread(buf,1U,sizeof(buf),index->fp)!=sizeof(buf))return false;
        ++index->record_reads;
        DiskRecord r={get_u64_le(buf),get_i64_le(buf+8)};
        if(r.key>high)break;
        if(produced>=out_capacity)return false;
        if(out)out[produced]=r;
        ++produced;
    }
    *out_count=produced;
    return true;
}

bool dsi_validate(DiskSortedIndex*index){
    if(!index||!index->fp)return false;
    size_t header_count=0U;
    if(!read_header(index->fp,&header_count)||header_count!=index->count)return false;

    off_t expected=HEADER_SIZE;
    if(index->count){
        off_t last=0;
        if(!record_offset(index->count-1U,&last))return false;
        expected=last+RECORD_SIZE;
    }
    if(fseeko(index->fp,0,SEEK_END)!=0)return false;
    off_t actual=ftello(index->fp);
    if(actual<0||actual!=expected)return false;

    uint64_t prev=0U;
    for(size_t i=0;i<index->count;++i){
        DiskRecord r;
        if(!read_record_at(index,i,&r,false))return false;
        if(i>0U&&prev>=r.key)return false;
        prev=r.key;
    }
    return true;
}
