# Pitfalls — R-Tree

- treating sibling MBRs as disjoint
- using integer width subtraction that overflows
- failing to update parent MBR after child insertion
- splitting without minimum-fill enforcement
- losing a propagated child split
- allowing leaves at different depths
- confusing object rectangles with internal MBR entries
- claiming overlap query is always logarithmic
