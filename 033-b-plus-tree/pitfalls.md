# Pitfalls — B+ Tree

- treating internal separator copy as logical duplicate record
- routing equality to wrong child
- leaf split not updating next link
- parent separator stale after deleting first key in leaf
- borrowing key but not refreshing separators
- internal borrow moving key instead of child under this representation
- merge breaking leaf chain
- root not shrinking
- range scan repeatedly searching root instead of following leaves
