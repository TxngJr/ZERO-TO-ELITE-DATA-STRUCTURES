# Invariants

1. Magic is exactly `DSR1`.
2. Version is exactly 1.
3. Header size is exactly 32.
4. Wire record size is exactly 20.
5. payload_size = record_count × 20 without overflow.
6. total blob length = 32 + payload_size exactly.
7. CRC32 covers payload bytes only.
8. Field byte order is big-endian.
9. Signed value bits survive round trip exactly.
10. Decoder allocates only after structural validation.
