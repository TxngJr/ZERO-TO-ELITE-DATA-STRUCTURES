# Visual Model

```text
clone:
x ─┐
   ├─> S refs=2 size=3 [10 20 30]
y ─┘

write y:
allocate T -> copy S -> S.refs-- -> y=T -> mutate T

x -> S [10 20 30]
y -> T [10 99 30]
```

Unique full storage ยังอาจต้อง allocate/copy เมื่อ capacity growth.
