# Chapter 087 — Posting List

A posting list is the ordered document-ID sequence attached to one indexed term. Sorted unique IDs enable fast Boolean operations and effective compression.
This implementation stores sorted unique uint32_t doc IDs, supports binary-search membership, two-pointer intersection, and gap/delta encoding with unsigned base-128 varints.
Gap encoding stores the first doc ID directly and later differences from the previous ID. Sorted IDs often produce much smaller integers than absolute document IDs.
The varint decoder rejects truncated, over-wide and non-increasing/overflowing gap streams.
