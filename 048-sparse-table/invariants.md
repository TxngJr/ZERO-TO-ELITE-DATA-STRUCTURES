# Invariants — IntSparseTable

1. logs[1]=0
2. logs[len]=floor(log2(len))
3. table[0][i]=a[i]
4. table[k][i] stores min over [i,i+2^k)
5. higher-level entry combines two adjacent half-size entries
6. only intervals fully inside [0,n) are meaningful
7. query uses two valid 2^k blocks covering the whole requested range
