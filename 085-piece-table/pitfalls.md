# Pitfalls

- treating add buffer as current document
- reclaiming deleted add bytes while pieces may reference them
- incorrect piece split offsets
- descriptor explosion without coalescing
- confusing byte offsets with Unicode character offsets
