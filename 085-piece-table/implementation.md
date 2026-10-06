# Implementation

Original is copied once and never modified. Add buffer is append-only logically. Insert splits at most one containing piece and adds a new ADD piece. Erase clips/removes overlapping pieces. Coalescing reduces descriptor fragmentation.
