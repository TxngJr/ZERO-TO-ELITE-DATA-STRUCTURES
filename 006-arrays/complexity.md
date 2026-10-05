# Complexity — Arrays

Let n = current size.

## Static array access

Index calculation uses base plus scaled offset:

    address(i) = base + i * element_size

under the RAM model this is Θ(1)

## Dynamic append

No grow:
    Θ(1)

Grow:
    allocate/copy n + one write
    Θ(n)

With geometric doubling over m appends:

    total copies < 2m
    total writes = m
    total Θ(m)

Therefore:
    Θ(1) amortized append

## Insert

Insert at i shifts n-i elements

    Θ(n-i)

Worst case i=0:
    Θ(n)

End insertion:
    Θ(1) excluding growth, amortized Θ(1) with doubling

## Erase

Erase i shifts n-i-1 elements:

    Θ(n-i)

Pop back:
    Θ(1)

## Search

Unsorted array linear search:
    best Θ(1)
    worst Θ(n)

Sorted array binary search:
    worst Θ(log n)

but sortedness maintenance may make insertion expensive

## Space

Built-in array n ints:
    Θ(n)

IntVector backing storage:
    Θ(capacity)

control metadata:
    Θ(1)

spare capacity:
    capacity-size elements of reserved space
