# Theory — Hash Set / Hash Map

## Set and Map Laws

Set:
    add(add(S,x),x) = add(S,x)

Map:
    put(put(M,k,v1),k,v2)
results in one key k mapped to v2.

These behavioral laws are independent of hashing.

## Probe Sequence Correctness

Lookup must follow the same probe rule insertion used.

For linear probing:
- stop at EMPTY
- continue through TOMBSTONE
- compare OCCUPIED keys

If insertion and lookup use different sequence, keys can become unreachable.

## Tombstone Pressure

Even with low live size, many tombstones can create long probe paths.

Therefore a table may rehash without increasing capacity simply to clean tombstones.

## String Equality

This map uses byte-wise strcmp semantics:
- case-sensitive
- null-terminated
- exact byte sequence equality

A Unicode-aware/case-insensitive map would need a different equality/hash contract.

## Borrowed vs Owned

API inputs are borrowed during call.
Stored keys become owned copies.

This distinction is crucial in C because pointer lifetime is not automatically managed.
