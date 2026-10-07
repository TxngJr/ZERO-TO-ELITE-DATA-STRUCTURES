# Invariants

1. Every alignment is a nonzero power of two.
2. field_offset % field_alignment == 0.
3. Fields never overlap.
4. struct_alignment = max field alignment.
5. struct_size % struct_alignment == 0.
6. struct_size includes all fields without overflow.
7. padding = struct_size - sum(field sizes).
8. aligned-array base % alignment == 0.
9. stride >= element_size.
10. stride % alignment == 0.
11. every element address = base + index × stride.
