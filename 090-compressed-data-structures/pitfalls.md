# Pitfalls

- quoting payload bytes while ignoring checkpoint metadata
- delta-encoding unsorted values
- allowing zero gaps in a unique sequence
- unchecked varint width/overflow
- choosing blocks too large for random access or too small for metadata efficiency
