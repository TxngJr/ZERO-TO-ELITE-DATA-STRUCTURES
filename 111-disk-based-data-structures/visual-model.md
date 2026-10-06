# Visual Model

```text
file
[ header 64B ]
[ key0 value0 ]
[ key1 value1 ]
[ key2 value2 ]
...

point lookup:
mid -> seek -> read
mid -> seek -> read
...

range:
binary lower_bound
      |
      v
one seek
read -> read -> read -> read sequentially
```
