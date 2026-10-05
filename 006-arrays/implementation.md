# Implementation — IntVector

## Representation

    struct IntVector {
        int *data;
        size_t size;
        size_t capacity;
    };

The public header keeps the struct opaque while exposing educational size/capacity accessors.

## Growth policy

Initial growth target:
    4 elements

Then:
    capacity *= 2

If doubling risks SIZE_MAX overflow, implementation falls back to the exact requested minimum after validation.

Before allocation:
    capacity <= SIZE_MAX / sizeof(int)

This prevents byte-size multiplication overflow.

## Push commit order

Correct order:

1. validate vector
2. check size+1 overflow
3. reserve storage
4. write data[size]
5. increment size

If reserve fails, logical state remains unchanged.

## Insert

For index <= size:
1. ensure capacity
2. memmove suffix one slot right
3. write new value
4. increment size

memmove is used instead of memcpy because source/destination ranges overlap.

## Erase

1. validate index
2. save removed value
3. memmove tail left
4. decrement size
5. optionally return removed value

## Clear

For int elements:
    size = 0

No per-element destructor exists

capacity remains for reuse.

## shrink_to_fit

If size=0:
    free storage

Otherwise realloc to exactly size elements

Failure leaves old vector valid.

## data pointer API

int_vector_data returns read-only pointer to backing elements for observation/interoperation.

Contract:
Any mutation that may reallocate can invalidate previously obtained pointer.

This is intentionally taught because iterator/reference invalidation is central to real dynamic arrays.
