# Implementation Notes

File integers are encoded explicitly little-endian instead of dumping C structs. This avoids dependence on compiler padding and host struct layout.

Header validation checks:
- exact magic
- version
- record size
- reserved bytes
- expected file length

`dsi_validate` sequentially checks strict key ordering without changing user-visible I/O counters.

Creation removes the target file on write/flush/close failure, but it is **not crash-atomic**: production storage engines need temp-file+rename/fsync or WAL/transactional protocols.
