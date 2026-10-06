# Visual Model

Without synchronization:

```text
A: find -> realloc -> shift -> size++
B:     find -> read old pointer -> inconsistent state
```

With mutex:

```text
A: lock | find -> grow -> shift -> publish | unlock
B:                                        lock | contains | unlock
```

Release/acquire:

```text
Producer                         Consumer
payload=42
store ready (release) ---------> load ready (acquire)
                                 read payload
```

Progress:
`blocking -> lock-free -> wait-free` are different guarantees, not marketing synonyms.
