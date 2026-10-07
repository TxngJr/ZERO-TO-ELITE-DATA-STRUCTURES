# Visual Model

```text
DsRecord[] in memory
        |
        | canonical encoder
        v
+-------------------------------+
| 32-byte header                |
+-------------------------------+
| record 0: 8 + 8 + 4 bytes    |
| record 1: 8 + 8 + 4 bytes    |
| ...                           |
+-------------------------------+
        |
        | validate + decoder
        v
new DsRecord[]
```

Memory padding never enters the byte stream.
