# Pitfalls — Strings

- forgetting null terminator
- buffer off-by-one: capacity bytes but no space for '\0'
- treating binary bytes as valid text
- assuming UTF-8 character = byte
- using strlen on non-terminated memory
- repeated concatenation causing quadratic copying
- keeping substring pointer after backing string reallocates
- aliasing source with destination during append/insert
- using memcpy for overlapping ranges
- integer overflow in new length + terminator
- comparing user text by raw bytes when locale/canonical equivalence is required
- assuming immutable abstraction means no internal sharing or allocation
