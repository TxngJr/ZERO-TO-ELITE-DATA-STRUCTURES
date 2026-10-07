# Implementation Notes

`MlDataset` stores:
- rows
- feature width
- contiguous float array
- labels
- per-row initialized bitmap

`BatchPlan` stores only row indices, batch size and cursor. Reset can either preserve current order or reshuffle with a new seed.

`EmbeddingTable` uses one contiguous matrix and per-row initialized metadata. Gather validates every index before copying that row.

All size products are overflow-checked before allocation.
