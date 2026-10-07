# Visual Model

```text
10.0.0.0/8 -> hop A
10.1.0.0/16 -> hop B
10.1.2.0/24 -> hop C

lookup 10.1.2.99
matches /8 -> /16 -> /24
answer = /24, hop C
```

Flow table:

```text
hash(5-tuple) -> slot
USED -> probe
TOMBSTONE -> remember possible insertion slot, continue search
EMPTY -> key absent / insertion boundary
```
