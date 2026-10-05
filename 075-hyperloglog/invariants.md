# Invariants — HyperLogLog

1. 4 <= p <= 18
2. m = 2^p
3. every register <= 65-p
4. add only increases a register
5. merge is component-wise maximum
6. equal-precision sketches are required for merge
