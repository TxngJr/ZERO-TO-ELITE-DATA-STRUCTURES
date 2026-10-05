# Invariants — IntVector

For every live valid vector:

1. size <= capacity
2. capacity == 0 implies data == NULL in this implementation
3. capacity > 0 implies data points to storage for at least capacity ints
4. logical elements occupy data[0..size)
5. indices >= size are not logical elements
6. size/capacity byte calculations do not overflow size_t before allocation

## Push preservation

If reserve succeeds:
    capacity >= old_size + 1

write at data[old_size] is inside allocation

then:
    new_size = old_size + 1 <= capacity

## Insert preservation

index <= old_size
suffix length = old_size-index

after shifting one slot right, destination ends at old_size, which is valid because capacity >= old_size+1

new logical sequence equals:
    old prefix + inserted value + old suffix

## Erase preservation

index < old_size

shift old suffix left one
new_size = old_size-1
new_size <= capacity remains true

## Reserve

Must never decrease size
Must preserve logical element order/content
Must establish capacity >= requested when returning true
