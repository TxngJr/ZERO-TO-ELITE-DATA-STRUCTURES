# Invariants — IntDaryHeap

1. d >= 2
2. size <= capacity
3. live indices dense in [0,size)
4. parent(i)=(i-1)/d
5. for every i>0:

       data[parent(i)] <= data[i]

6. root is global minimum when non-empty
