# Visual Model

```text
free block [offset=100,size=200]

request size=80 alignment causes aligned start=128

prefix  = [100,28]
alloc   = [128,80]
suffix  = [208,92]

release alloc:
[100,28] [128,80] [208,92]
   merge left + current + right
=> [100,200]
```
