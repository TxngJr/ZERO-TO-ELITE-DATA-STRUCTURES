# Visual Model

Dataset:

```text
X:
row0 [....]
row1 [....]
row2 [....]
row3 [....]

order after shuffle:
[2,0,3,1]

batch size 2:
batch0 -> rows [2,0]
batch1 -> rows [3,1]
```

Embedding:

```text
token id -> embedding row
  17     -> [e0 e1 ... e31]
```

Index arrays express logical order while storage remains contiguous.
