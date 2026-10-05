#ifndef BYTE_STRING_H
#define BYTE_STRING_H

#include <stdbool.h>
#include <stddef.h>

typedef struct ByteString ByteString;

ByteString *byte_string_create(void);
ByteString *byte_string_from_bytes(const char *bytes, size_t count);
void byte_string_free(ByteString *string);

size_t byte_string_length(const ByteString *string);
size_t byte_string_capacity(const ByteString *string);
const char *byte_string_c_str(const ByteString *string);

bool byte_string_reserve(ByteString *string, size_t min_capacity);
bool byte_string_append_char(ByteString *string, char ch);
bool byte_string_append_bytes(ByteString *string, const char *bytes, size_t count);
bool byte_string_insert_bytes(ByteString *string, size_t index, const char *bytes, size_t count);
bool byte_string_erase(ByteString *string, size_t index, size_t count);
void byte_string_clear(ByteString *string);

ptrdiff_t byte_string_find(const ByteString *string, const char *needle, size_t needle_len);

#endif
