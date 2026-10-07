# Implementation Notes

Alignments must be nonzero powers of two.

`ma_align_up` uses:
- overflow check for `value + alignment - 1`
- bit-mask rounding

Layout calculator checks every aligned offset plus field size before addition.

Aligned array avoids relying on C17 `aligned_alloc`. It allocates `logical_bytes + alignment - 1`, rounds the raw address upward with `uintptr_t`, and retains the original pointer for `free`.

Stride is `align_up(element_size, alignment)`, guaranteeing every element base has the requested alignment when base is aligned.
