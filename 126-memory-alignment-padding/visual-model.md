# Visual Model

Natural layout:

```text
offset
0       [u8]
1..7    [padding]
8..15   [u64]
16..19  [u32]
20..23  [tail padding]
```

Aligned array:

```text
64-byte boundary
| element0 payload .... padding |
| element1 payload .... padding |
| element2 payload .... padding |
```

Each element starts at `base + i * stride`.
