# Theory — Bloom Filter

Each inserted key sets k bit positions.

Membership query checks the same positions:
- if any bit is zero, key definitely was not inserted
- if all are one, those bits may have been set by this key or by collisions from other keys

That asymmetry creates no-false-negative insert-only semantics but permits false positives.
