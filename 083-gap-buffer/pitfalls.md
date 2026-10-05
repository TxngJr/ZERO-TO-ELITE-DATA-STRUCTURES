# Pitfalls

- treating physical gap bytes as document bytes
- memcpy on overlapping gap moves instead of memmove
- UTF-8 code-point splitting
- resizing without relocating suffix correctly
- assuming distant edits are O(1)
