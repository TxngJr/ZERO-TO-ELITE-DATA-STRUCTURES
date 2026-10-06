# Implementation

Shared:
- `data[]` fixed after creation
- atomic `head`
- atomic `tail`
- immutable capacity

Producer is sole writer of tail and data slot before publication.
Consumer is sole writer of head and reads a slot only after acquire observation of tail.

Release on head tells producer that consumed slot may be reused; producer acquire-loads head before overwriting a wrapped slot.

Object destruction remains external lifetime responsibility.
