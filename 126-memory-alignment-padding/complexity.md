# Complexity

Let F = field count, N = aligned-array elements.

| Operation | Time |
|---|---:|
| compute layout | Θ(F) |
| align_up | Θ(1) |
| create aligned array | Θ(1) allocation |
| element address | Θ(1) |
| validate array metadata/addresses | Θ(N) |

Aligned-array space = N × stride + at most alignment-1 allocator slack.
