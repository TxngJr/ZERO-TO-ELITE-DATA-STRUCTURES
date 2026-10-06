# Visual Model

```text
roots
  |
  v
 [A] -> [B] -> [C]

 [X] <-> [Y]     no root path

mark: A,B,C
sweep: reclaim X,Y
```

Generation:

```text
slot 7 generation 3 -> swept
slot 7 generation 4 -> reused

old handle {7,3} != current {7,4}
```
