# Implementation

Metadata: capacity, gap_start, gap_end. Logical size = capacity - gap_size. Move-gap uses memmove; growth allocates a larger array and places suffix at the new right edge.
