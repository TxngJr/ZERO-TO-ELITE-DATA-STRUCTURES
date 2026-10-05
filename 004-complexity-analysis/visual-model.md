# Visual Model — Growth

## Linear

n=1   *
n=2   **
n=4   ****
n=8   ********

doubling n roughly doubles dominant work

## Quadratic

n=1   *
n=2   ****
n=4   ****************
n=8   ****************************************************************

doubling n roughly quadruples dominant work

## Logarithmic by repeated halving

    64
     |
    32
     |
    16
     |
     8
     |
     4
     |
     2
     |
     1

6 halvings because log2(64)=6

## Dynamic-array doubling

    capacity: 1 -> 2 -> 4 -> 8 -> 16

copy events before capacity 16:

    1 + 2 + 4 + 8 = 15

expensive operations are sparse across many cheap appends

## Four lenses

    source code
       |
       +--> exact operation count
       +--> asymptotic growth
       +--> empirical benchmark
       +--> hardware profiling

ไม่มี lens เดียวแทนทั้งหมด
