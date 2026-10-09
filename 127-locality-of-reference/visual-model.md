# Visual Model

8-byte elements, 64-byte line:

```text
line 0: element 0 1 2 3 4 5 6 7
line 1: element 8 9 10 11 12 13 14 15
...
```

Sequential:

```text
0 1 2 3 4 5 6 7 | 8 9 10 ...
^^^^^^^^^^^^^^^     spatial reuse inside line
```

Stride 8:

```text
0 | 8 | 16 | 24 | ...
new line almost every access
```

Repeated scan:

```text
0..127, 0..127
         ^^^^^^ temporal line reuse
```
