# Implementation — IntStack

## Public API

The header exposes only an incomplete type:

    typedef struct IntStack IntStack;

Users receive/create a pointer and call operations. They cannot legally access fields because the struct body lives in int_stack.c.

## Representation

    struct IntStack {
        int *data;
        size_t size;
        size_t capacity;
    };

## Growth

When push needs more capacity:
1. choose initial capacity or double current capacity
2. check multiplication/size overflow
3. call realloc into a temporary pointer
4. on failure, return false without committing a broken pointer
5. on success, update data/capacity
6. write value
7. increment size

## Ownership

IntStack owns data.
int_stack_free releases data and then the IntStack object.

Caller owns only the handle returned by create in the sense that it is responsible for eventually calling int_stack_free.

## Why array-backed here?

The goal is to expose the difference between ADT and representation. The same public Stack behavior could later be implemented with linked nodes.
