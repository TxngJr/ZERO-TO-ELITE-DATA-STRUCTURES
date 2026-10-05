# Invariants — IntIntervalHeap

1. all nodes except possibly final node contain two logical elements
2. if size is odd, final singleton has low==high
3. every node satisfies low<=high
4. parent.low<=child.low
5. child.high<=parent.high
6. root.low is global minimum
7. root.high is global maximum
8. complete-tree shape follows array indexing
