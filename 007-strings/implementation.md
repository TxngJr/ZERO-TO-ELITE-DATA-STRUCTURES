# Implementation — ByteString

ByteString is intentionally byte-oriented.

## Representation

    struct ByteString {
        char *data;
        size_t length;
        size_t capacity;
    };

capacity = maximum logical bytes before another growth.
allocation size = capacity + 1 for trailing zero.

## Operations

- create / free
- length / capacity / c_str
- reserve
- append_char
- append_bytes
- insert_bytes
- erase
- clear
- find

## Overlap-safe append/insert

If source pointer aliases the current backing buffer and reserve reallocates, the old source pointer could become invalid.

Implementation detects source offset when source lies inside current logical buffer, performs reserve, then reconstructs source from the new data pointer before memmove.

This is an important real-world aliasing problem often hidden in simplistic dynamic-string examples.

## Growth

Initial capacity 16, then geometric doubling.
Overflow checks cover:
- length + count
- capacity + terminator
- doubling

## Terminator invariant

Every successful mutation ends with:

    data[length] = '\0'

Even though the abstraction is byte-oriented, this enables C-string interoperability when content contains no embedded zeros.
