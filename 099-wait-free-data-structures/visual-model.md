# Visual Model

```text
physical capacity = 8
usable = 7

      head
       v
[ A ][ B ][ C ][   ][   ][   ][   ][   ]
                 ^
                tail

producer writes slot then release-publishes tail
consumer acquire-loads tail then reads slot
```

No CAS retry inside either try operation.
