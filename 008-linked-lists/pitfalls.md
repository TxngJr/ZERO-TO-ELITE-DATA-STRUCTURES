# Pitfalls — Linked Lists

- forgetting to update tail when first/last node changes
- freeing node before reading its next pointer
- leaving predecessor pointing to freed node
- doubly list next/prev disagreement
- decrementing size on failed erase
- circular destructor looking for NULL forever
- losing entire suffix by overwriting next too early
- not handling one-element list separately/correctly
- claiming insertion O(1) when API first searches by index
- memory leak from abandoned nodes
- iterator/raw-node lifetime after erase
- poor cache locality and allocator overhead ignored in design choice
- accidental cycle in a supposedly linear list
