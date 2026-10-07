# Implementation Notes

All integers use explicit big-endian load/store helpers.

Signed `int64_t value` is converted to/from a `uint64_t` bit container with `memcpy`. This preserves the object representation bits without relying on an out-of-range unsigned-to-signed cast.

CRC32 uses the standard reflected polynomial `0xEDB88320`. The public helper permits testing against the well-known `123456789 -> CBF43926` vector.

Deserializer validates the complete header before allocation and requires exact blob length.
