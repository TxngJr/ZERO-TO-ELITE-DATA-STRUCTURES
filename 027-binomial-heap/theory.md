# Theory — Binomial Heap

## Binary-counter analogy

At most one B_k exists per degree.

Insertion can cause:

    B0+B0 -> B1
    B1+B1 -> B2
    B2+B2 -> B3

เหมือน carry bits ของ binary addition.

## Number of roots

Root degree set corresponds to set bits of n.

Therefore number of roots <= floor(log2 n)+1.

## B_k size proof

Base:
    B0 has 1=2^0 node.

Induction:
    B_k links two B_(k-1).

So:

    size(B_k)=2*2^(k-1)=2^k

## Link correctness

Original trees already satisfy heap order.

Choose smaller root as parent.

Only new edge must satisfy:

    parent.key <= child.key

therefore linked tree remains heap-ordered.
