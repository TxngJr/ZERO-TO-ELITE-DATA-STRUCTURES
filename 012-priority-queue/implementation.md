# Implementation — MaxPriorityQueue

## Item

    typedef struct {
        int value;
        int priority;
    } PriorityItem;

Tie order is unspecified.

## Representation

    PriorityItem *data
    size_t size
    size_t capacity

Root:
    data[0]

## Push

1. grow backing array geometrically if needed
2. append item at size
3. size++
4. sift_up(last index)

## Pop

1. copy root
2. move last item to root
3. size--
4. if non-empty, sift_down(0)

## Comparator

Higher integer priority wins.

Equal priority:
no swap required solely from equality.
Relative order can still change through other swaps, so API does not promise stability.

## Validation

For each i:
- if left child exists, parent priority >= left priority
- if right child exists, parent priority >= right priority

Also checks size/capacity metadata.

## Overflow Safety

Growth validates:
    capacity <= SIZE_MAX / sizeof(PriorityItem)

Child indexing in sift-down is formed only while parent is in a range where a left child may exist:

    index < size/2

This avoids blindly computing 2*index+1 for arbitrary huge index.
