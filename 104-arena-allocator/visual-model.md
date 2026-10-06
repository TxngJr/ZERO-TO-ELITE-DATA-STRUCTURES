# Visual Model

```text
mark M
block A: [old data.....|used]
                       ^
                       M

allocate more
block B: [new data......]
block C: [new data...]

reset_to_mark(M)
free C
free B
restore block A used
```

หลัง reset generation เปลี่ยน ดังนั้น marker เก่าถูก invalidate.
