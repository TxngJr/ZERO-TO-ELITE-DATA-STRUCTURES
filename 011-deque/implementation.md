# Implementation — IntDeque

## Fields

    int *data
    size_t capacity
    size_t head
    size_t size

## Helpers

next(index):
    index+1 unless last, then 0

prev(index):
    capacity-1 if index==0, else index-1

wrapped logical offset:
    advance head by offset without unchecked head+offset overflow

## Push Front

1. ensure capacity
2. move head backward one slot
3. write
4. size++

Special empty case still works after initial allocation because head starts 0 and previous(0) picks last slot.

## Push Back

1. ensure capacity
2. locate wrap(head+size)
3. write
4. size++

## Pop

Pop front advances head.
Pop back only reduces size.

When deque becomes empty, head resets to 0 to maintain canonical empty state.

## Monotonic Queue Implementation

sliding_window_maximum uses a raw index buffer of size n as an internal deque.

This avoids coupling algorithm correctness to IntDeque's int-value API, since indices are size_t.
