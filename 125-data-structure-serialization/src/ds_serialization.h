#ifndef DS_SERIALIZATION_H
#define DS_SERIALIZATION_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
typedef struct {
    uint64_t id;
    int64_t value;
    uint32_t flags;
} DsRecord;
uint32_t ds_crc32(const void *data,size_t len);
bool ds_serialized_size(size_t record_count,size_t *out_size);
bool ds_serialize(const DsRecord *records,size_t record_count,unsigned char *out,size_t out_capacity,size_t *out_size);
DsRecord *ds_deserialize(const void *data,size_t len,size_t *out_count);
bool ds_validate_blob(const void *data,size_t len);
#endif
