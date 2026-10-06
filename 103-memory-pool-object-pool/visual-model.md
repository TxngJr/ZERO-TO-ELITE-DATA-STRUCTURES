# Visual Model

```text
allocate()
free_top--
idx = free_stack[free_top]
in_use[idx] = 1
return storage + idx*stride

release(ptr)
ptr -> exact slot index
reject foreign/misaligned/double-free
in_use[idx] = 0
free_stack[free_top++] = idx
```

Logical object size กับ physical stride อาจไม่เท่ากันเพราะ alignment padding.
