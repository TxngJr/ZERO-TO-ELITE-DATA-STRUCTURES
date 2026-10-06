# Theory — Tensor Layout

For index i[d], physical element offset = base + sum(i[d] * stride[d]). Row-major makes the last dimension stride 1; column-major makes the first dimension stride 1.
Transpose/permute can often be metadata-only. Slice with step>1 increases one stride and becomes non-contiguous.
