# Theory — Strings

## String is an abstraction, encoding is a representation choice

A logical text value may be represented in:
- ASCII-compatible bytes
- UTF-8
- UTF-16
- UTF-32
- language/runtime-specific compact forms

Do not infer "one character = one byte".

## C string contract

A pointer passed to a C string function generally must designate a sequence that contains a terminating zero byte within valid accessible storage according to that API's contract.

The pointer itself does not carry length.

## Length-prefixed / length-aware strings

If length is stored:
- size query can be constant-time
- embedded zero bytes can be represented
- substring views can carry pointer+length

A view must not outlive backing storage.

## Immutability

Immutable values allow sharing:
multiple references can safely observe same logical value if runtime/language memory model supports it.

But implementation may still use:
- copy-on-write
- interning
- ropes
- compact encodings

These are implementation choices, not implied by the word immutable.

## String interning

Interning stores one canonical instance for repeated equal values.

Trade-offs:
- faster identity-like comparisons after canonicalization
- memory saving for repeated strings
- table management cost
- lifetime/GC complexity

Useful in compilers/symbol tables and runtimes.

## Small String Optimization

Some C++ string implementations may store short content inside the object to avoid heap allocation.

Important:
- exact size is implementation-specific
- not a portable guarantee to rely on unless documented by implementation/ABI

The concept teaches representation trade-offs between metadata/object size and allocation reduction.
