# Pitfalls — Bitmap

- incrementing cardinality when adding an existing member
- decrementing when removing an absent member
- forgetting padding mask after difference/complement-like operations
- range mask off-by-one
- shifting by 64
- using dense bitmap for gigantic sparse universe
- confusing cardinality with universe size
