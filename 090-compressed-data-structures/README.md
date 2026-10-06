# Chapter 090 — Compressed Data Structures

Compressed data structures reduce representation size while retaining useful operations without always expanding the whole dataset first.
This chapter implements a strictly increasing uint64 sequence using block checkpoints plus delta-varint payloads. Each block stores its first absolute value and byte offset; later values in the block are positive gaps encoded as base-128 varints.
Random access decodes only from the nearest block checkpoint. lower_bound first searches block bases, then scans one block.
