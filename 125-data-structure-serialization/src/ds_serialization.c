#include "ds_serialization.h"
#include <stdlib.h>
#include <string.h>

enum {
    DS_HEADER_SIZE=32,
    DS_WIRE_RECORD_SIZE=20,
    DS_VERSION=1
};

static void store_be16(unsigned char*p,uint16_t v){
    p[0]=(unsigned char)(v>>8U);
    p[1]=(unsigned char)v;
}
static void store_be32(unsigned char*p,uint32_t v){
    p[0]=(unsigned char)(v>>24U);
    p[1]=(unsigned char)(v>>16U);
    p[2]=(unsigned char)(v>>8U);
    p[3]=(unsigned char)v;
}
static void store_be64(unsigned char*p,uint64_t v){
    for(unsigned i=0;i<8U;++i)
        p[7U-i]=(unsigned char)(v>>(i*8U));
}
static uint16_t load_be16(const unsigned char*p){
    return (uint16_t)(((uint16_t)p[0]<<8U)|(uint16_t)p[1]);
}
static uint32_t load_be32(const unsigned char*p){
    return ((uint32_t)p[0]<<24U)|((uint32_t)p[1]<<16U)|
           ((uint32_t)p[2]<<8U)|(uint32_t)p[3];
}
static uint64_t load_be64(const unsigned char*p){
    uint64_t v=0U;
    for(unsigned i=0;i<8U;++i)v=(v<<8U)|p[i];
    return v;
}

uint32_t ds_crc32(const void*data,size_t len){
    if(!data&&len)return 0U;
    const unsigned char*p=data;
    uint32_t crc=UINT32_C(0xffffffff);
    for(size_t i=0;i<len;++i){
        crc^=p[i];
        for(unsigned bit=0;bit<8U;++bit)
            crc=(crc>>1U)^(UINT32_C(0xedb88320)&(uint32_t)-(int32_t)(crc&1U));
    }
    return crc^UINT32_C(0xffffffff);
}

bool ds_serialized_size(size_t count,size_t*out_size){
    if(!out_size||count>SIZE_MAX/DS_WIRE_RECORD_SIZE)return false;
    size_t payload=count*DS_WIRE_RECORD_SIZE;
    if(payload>SIZE_MAX-DS_HEADER_SIZE)return false;
    *out_size=DS_HEADER_SIZE+payload;
    return true;
}

static void encode_record(const DsRecord*r,unsigned char*out){
    uint64_t value_bits=0U;
    memcpy(&value_bits,&r->value,sizeof(value_bits));
    store_be64(out,r->id);
    store_be64(out+8U,value_bits);
    store_be32(out+16U,r->flags);
}

bool ds_serialize(const DsRecord*records,size_t count,unsigned char*out,
                  size_t capacity,size_t*out_size){
    if(!out_size||(count&&(!records||!out)))return false;
    size_t total=0U;
    if(!ds_serialized_size(count,&total)||capacity<total)return false;
    if(!out&&total)return false;

    memcpy(out,"DSR1",4U);
    store_be16(out+4U,DS_VERSION);
    store_be16(out+6U,DS_HEADER_SIZE);
    store_be64(out+8U,(uint64_t)count);
    store_be32(out+16U,DS_WIRE_RECORD_SIZE);
    store_be64(out+20U,(uint64_t)(total-DS_HEADER_SIZE));

    unsigned char*payload=out+DS_HEADER_SIZE;
    for(size_t i=0;i<count;++i)
        encode_record(&records[i],payload+i*DS_WIRE_RECORD_SIZE);
    store_be32(out+28U,ds_crc32(payload,total-DS_HEADER_SIZE));
    *out_size=total;
    return true;
}

typedef struct {
    size_t count;
    size_t payload;
} ParsedHeader;

static bool parse_header(const unsigned char*data,size_t len,ParsedHeader*out){
    if(!data||!out||len<DS_HEADER_SIZE)return false;
    if(memcmp(data,"DSR1",4U)!=0)return false;
    if(load_be16(data+4U)!=DS_VERSION||
       load_be16(data+6U)!=DS_HEADER_SIZE||
       load_be32(data+16U)!=DS_WIRE_RECORD_SIZE)return false;

    uint64_t count64=load_be64(data+8U);
    uint64_t payload64=load_be64(data+20U);
    if(count64>SIZE_MAX||payload64>SIZE_MAX)return false;
    size_t count=(size_t)count64;
    size_t payload=(size_t)payload64;
    if(count>SIZE_MAX/DS_WIRE_RECORD_SIZE||
       payload!=count*DS_WIRE_RECORD_SIZE||
       payload>SIZE_MAX-DS_HEADER_SIZE||
       len!=DS_HEADER_SIZE+payload)return false;

    uint32_t expected=load_be32(data+28U);
    uint32_t actual=ds_crc32(data+DS_HEADER_SIZE,payload);
    if(expected!=actual)return false;
    out->count=count;
    out->payload=payload;
    return true;
}

bool ds_validate_blob(const void*data,size_t len){
    ParsedHeader h;
    return parse_header(data,len,&h);
}

DsRecord *ds_deserialize(const void*data,size_t len,size_t*out_count){
    if(!out_count)return NULL;
    const unsigned char*bytes=data;
    ParsedHeader h;
    if(!parse_header(bytes,len,&h))return NULL;
    if(h.count>SIZE_MAX/sizeof(DsRecord))return NULL;

    DsRecord*records=h.count?malloc(h.count*sizeof(*records)):calloc(1,1U);
    if(!records)return NULL;
    const unsigned char*payload=bytes+DS_HEADER_SIZE;
    for(size_t i=0;i<h.count;++i){
        const unsigned char*p=payload+i*DS_WIRE_RECORD_SIZE;
        records[i].id=load_be64(p);
        uint64_t value_bits=load_be64(p+8U);
        memcpy(&records[i].value,&value_bits,sizeof(value_bits));
        records[i].flags=load_be32(p+16U);
    }
    *out_count=h.count;
    return records;
}
