# Implementation — IntIntervalHeap

Node:
- int64_t low
- int64_t high

Heap:
- node array
- element count
- node capacity

Singleton:
    final node when size is odd
    low == high

Insertion:
- completes singleton or creates new singleton
- sifts only one endpoint side upward

Deletion:
- removes root endpoint
- extracts replacement from final node
- sifts through corresponding side
- repairs low/high relation when crossing the opposite endpoint
