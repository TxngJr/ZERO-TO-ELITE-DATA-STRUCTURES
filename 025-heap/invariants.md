# Invariants — IntMinHeap

1. size <= capacity
2. data is non-NULL when capacity > 0
3. live elements occupy dense indices [0,size)
4. complete-tree shape follows from dense indexing
5. for every i>0:

       data[parent(i)] <= data[i]

6. when size>0, data[0] is global minimum
