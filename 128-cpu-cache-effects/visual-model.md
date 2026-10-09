# Visual Model

Example: 4 sets, 2 ways

```text
set 0: [MRU tag 9] [LRU tag 2]
set 1: [tag 4]     [tag 1]
set 2: [...]
set 3: [...]
```

Access mapping:

```text
address
  |
 / line_size
  v
line number
  |
  +--> line % sets = set
  |
  +--> line / sets = tag
```

Hit moves tag to MRU.
Full-set miss drops LRU.
