# Visual Model

Atomic bit update:

```text
old word
   |
fetch_or(mask)
   |
new word
```

Tagged CAS:

```text
snapshot = (value=7, version=0)
7/0 -> 9/1 -> 7/2

stale CAS expecting 7/0
current is 7/2
=> fail despite same value
```
