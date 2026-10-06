# Visual Model

```text
head -> A -> B -> C

push X:
X.next=A
CAS(head,A,X)

two threads:
T1 reads A
T2 CAS A->B succeeds
T1 CAS A->X fails, observed updates, retry
```

Lifetime pool:

```text
slot0 push once
slot1 push once
...
popped slots remain allocated, never recycled
destroy -> free entire pool
```
