# Complexity

Let N = process capacity and Q = selected ready-queue length.

| Operation | Time |
|---|---:|
| spawn | Θ(1) |
| dispatch | Θ(1) with 4 fixed priorities |
| yield | Θ(1) |
| block | Θ(1) |
| wake | Θ(1) |
| terminate blocked/running | Θ(1) |
| terminate ready | O(Q) |
| change ready priority | O(Q) |
| state lookup | Θ(1) |
| validate | Θ(N) |

Space is Θ(N).
