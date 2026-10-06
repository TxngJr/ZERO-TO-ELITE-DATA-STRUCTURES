# Chapter 089 — Matrix / Tensor Storage Layout

A tensor's logical shape is separate from its physical layout. Strides tell how many storage elements to skip when one logical index advances by one.
This chapter models layouts without owning data: ndim, shape, stride and base offset. It constructs row-major and column-major contiguous layouts, permutes axes, creates positive-step slices and computes physical offsets safely.
Views can be non-contiguous even though they share the same underlying storage.
