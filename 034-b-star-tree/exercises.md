# Exercises — Chapter 034

## Beginner
1. What does B* improve over B-Tree?
2. What happens before split?
3. What is a 2-to-3 split?
4. Why root has exceptions?
5. What is order m?

## Intermediate
6. Compute max/min keys for m=6.
7. Trace sibling redistribution.
8. Trace a 2-to-3 split.
9. Compare B-Tree and B* occupancy.
10. Explain page utilization benefit.

## Advanced
11. Prove redistribution preserves sorted order.
12. Derive non-root occupancy for m divisible by 3.
13. Explain why ordinary 2-node merge is insufficient for deletion.
14. Design a 3-to-2 deletion repair.
15. Analyze write amplification of redistribution vs split.

## Implementation
16. Add binary search inside nodes.
17. Count redistributions and 2-to-3 splits.
18. Add page-byte utilization model.

## Challenge / Research
19. Implement logarithmic B* deletion with multi-sibling repair.
20. Compare B*, B+ and prefix-compressed database pages.
