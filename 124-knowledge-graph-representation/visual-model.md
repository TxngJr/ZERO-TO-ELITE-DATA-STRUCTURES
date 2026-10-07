# Visual Model

```text
"alice" --intern--> 17
"knows" --intern--> 42
"bob"   --intern--> 91

triple:
(17, 42, 91)

same triple appears in indexes:

SPO: (subject,predicate,object)
POS: (predicate,object,subject)
OSP: (object,subject,predicate)
```

Query `(17, ?, ?)` chooses SPO.
Query `(?, 42, ?)` chooses POS.
Query `(?, ?, 91)` chooses OSP.
