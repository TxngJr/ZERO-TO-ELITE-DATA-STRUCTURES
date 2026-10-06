# Visual Model

```text
Class 32
  slab A: [used][free][used][free]...
  slab B: [full][full][full]...

Class 64
  slab C: [used][used][free]...

request 25 -> class 32
request 40 -> class 64
```

Internal fragmentation ของ request 25 ใน class 32 = 7 bytes.
