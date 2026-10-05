# Complexity Preview — IntStack

Formal proof and notation come in Chapter 004.

Operation reasoning:

| Operation | Work intuition |
|---|---|
| is_empty | reads size |
| size | reads size |
| peek | checks size and reads last element |
| pop | checks size, decrements it, reads last element |
| push without growth | writes one element |
| push with growth | may allocate/move/copy existing elements |

Therefore push has two different cost shapes:
- common push when capacity remains
- occasional expensive growth

Later:
- Chapter 004 formalizes Big-O/Omega/Theta
- Chapter 006 analyzes dynamic arrays
- Chapter 131 revisits amortized data structures

Do not claim "push is always constant time" from the source code alone.
