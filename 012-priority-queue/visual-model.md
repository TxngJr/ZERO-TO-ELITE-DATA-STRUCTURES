# Visual Model — Priority Queue

## Max heap tree

          (A,100)
         /       \
     (B,70)     (C,90)
      /   \       /
  (D,20) (E,60) (F,50)

Array:

index  0       1      2      3      4      5
      A100    B70    C90    D20    E60    F50

Parent dominates children.

## Push (X,95)

Append:

    [100,70,90,20,60,50,95]

X parent is 90:
95 > 90 => swap

    [100,70,95,20,60,50,90]

Now parent 100 dominates 95: stop.

## Pop Max

remove 100
move last 90 to root:

    [90,70,95,20,60,50]

larger child is 95:
swap:

    [95,70,90,20,60,50]

invariant restored.
