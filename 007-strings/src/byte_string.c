#include "byte_string.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

struct ByteString {
    char *data;
    size_t length;
    size_t capacity;
};

static bool allocation_bytes(size_t capacity, size_t *bytes) {
    if (bytes == NULL || capacity == SIZE_MAX) {
        return false;
    }
    *bytes = capacity + 1;
    return true;
}

ByteString *byte_string_create(void) {
    ByteString *s = calloc(1, sizeof *s);
    if (s == NULL) {
        return NULL;
    }

    s->data = malloc(1);
    if (s->data == NULL) {
        free(s);
        return NULL;
    }

    s->data[0] = '\0';
    return s;
}

ByteString *byte_string_from_bytes(const char *bytes, size_t count) {
    if (bytes == NULL && count != 0) {
        return NULL;
    }

    ByteString *s = byte_string_create();
    if (s == NULL) {
        return NULL;
    }

    if (!byte_string_append_bytes(s, bytes, count)) {
        byte_string_free(s);
        return NULL;
    }

    return s;
}

void byte_string_free(ByteString *string) {
    if (string == NULL) {
        return;
    }

    free(string->data);
    free(string);
}

size_t byte_string_length(const ByteString *string) {
    return string == NULL ? 0 : string->length;
}

size_t byte_string_capacity(const ByteString *string) {
    return string == NULL ? 0 : string->capacity;
}

const char *byte_string_c_str(const ByteString *string) {
    return string == NULL ? NULL : string->data;
}

bool byte_string_reserve(ByteString *string, size_t min_capacity) {
    if (string == NULL) {
        return false;
    }

    if (min_capacity <= string->capacity) {
        return true;
    }

    size_t new_capacity = string->capacity == 0 ? 16 : string->capacity;

    while (new_capacity < min_capacity) {
        if (new_capacity > (SIZE_MAX - 1) / 2) {
            new_capacity = min_capacity;
            break;
        }
        new_capacity *= 2;
    }

    size_t bytes = 0;
    if (!allocation_bytes(new_capacity, &bytes)) {
        return false;
    }

    char *new_data = realloc(string->data, bytes);
    if (new_data == NULL) {
        return false;
    }

    string->data = new_data;
    string->capacity = new_capacity;
    return true;
}

static bool source_offset_if_aliased(
    const ByteString *string,
    const char *bytes,
    size_t count,
    size_t *offset,
    bool *aliased
) {
    if (string == NULL || offset == NULL || aliased == NULL) {
        return false;
    }

    *offset = 0;
    *aliased = false;

    if (count == 0 || bytes == NULL) {
        return true;
    }

    const uintptr_t base = (uintptr_t)(const void *)string->data;
    const uintptr_t source = (uintptr_t)(const void *)bytes;

    if (source >= base && source - base <= string->length) {
        const size_t off = (size_t)(source - base);

        if (count > string->length - off) {
            return false;
        }

        *offset = off;
        *aliased = true;
    }

    return true;
}

bool byte_string_append_char(ByteString *string, char ch) {
    if (string == NULL || string->length == SIZE_MAX) {
        return false;
    }

    const size_t new_length = string->length + 1;
    if (!byte_string_reserve(string, new_length)) {
        return false;
    }

    string->data[string->length] = ch;
    string->length = new_length;
    string->data[string->length] = '\0';
    return true;
}

bool byte_string_append_bytes(ByteString *string, const char *bytes, size_t count) {
    if (string == NULL || (bytes == NULL && count != 0)) {
        return false;
    }

    if (count == 0) {
        return true;
    }

    if (count > SIZE_MAX - string->length) {
        return false;
    }

    size_t offset = 0;
    bool aliased = false;
    if (!source_offset_if_aliased(string, bytes, count, &offset, &aliased)) {
        return false;
    }

    const size_t old_length = string->length;
    const size_t new_length = old_length + count;

    if (!byte_string_reserve(string, new_length)) {
        return false;
    }

    const char *source = aliased ? string->data + offset : bytes;
    memmove(string->data + old_length, source, count);

    string->length = new_length;
    string->data[string->length] = '\0';
    return true;
}

bool byte_string_insert_bytes(
    ByteString *string,
    size_t index,
    const char *bytes,
    size_t count
) {
    if (string == NULL || index > string->length || (bytes == NULL && count != 0)) {
        return false;
    }

    if (count == 0) {
        return true;
    }

    if (count > SIZE_MAX - string->length) {
        return false;
    }

    size_t offset = 0;
    bool aliased = false;
    if (!source_offset_if_aliased(string, bytes, count, &offset, &aliased)) {
        return false;
    }

    char *temporary = NULL;
    if (aliased) {
        temporary = malloc(count);
        if (temporary == NULL) {
            return false;
        }

        memcpy(temporary, string->data + offset, count);
        bytes = temporary;
    }

    const size_t old_length = string->length;
    const size_t new_length = old_length + count;

    if (!byte_string_reserve(string, new_length)) {
        free(temporary);
        return false;
    }

    memmove(
        string->data + index + count,
        string->data + index,
        old_length - index + 1
    );

    memcpy(string->data + index, bytes, count);

    string->length = new_length;
    string->data[string->length] = '\0';

    free(temporary);
    return true;
}

bool byte_string_erase(ByteString *string, size_t index, size_t count) {
    if (string == NULL || index > string->length) {
        return false;
    }

    if (index == string->length || count == 0) {
        return true;
    }

    const size_t available = string->length - index;
    if (count > available) {
        count = available;
    }

    memmove(
        string->data + index,
        string->data + index + count,
        string->length - index - count + 1
    );

    string->length -= count;
    return true;
}

void byte_string_clear(ByteString *string) {
    if (string != NULL) {
        string->length = 0;
        string->data[0] = '\0';
    }
}

ptrdiff_t byte_string_find(
    const ByteString *string,
    const char *needle,
    size_t needle_len
) {
    if (string == NULL || (needle == NULL && needle_len != 0)) {
        return -1;
    }

    if (needle_len == 0) {
        return 0;
    }

    if (needle_len > string->length) {
        return -1;
    }

    const size_t last = string->length - needle_len;
    for (size_t i = 0; i <= last; ++i) {
        if (memcmp(string->data + i, needle, needle_len) == 0) {
            return (ptrdiff_t)i;
        }
    }

    return -1;
}
