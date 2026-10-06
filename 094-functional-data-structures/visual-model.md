# Visual Model

```text
q1: rear -> [10]
q2: rear -> [20] -> [10]
                    ^
                    shared by q1

dequeue(q2):
rear [20]->[10] --reverse-copy--> front [10']->[20']
return 10
```
