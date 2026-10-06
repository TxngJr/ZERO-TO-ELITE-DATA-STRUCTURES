# Complexity

Let N = records and K = range output size.

| Operation | CPU / record work | File access pattern |
|---|---:|---|
| create | Θ(N) | sequential writes |
| open | Θ(1) | header read |
| get | O(log N) | O(log N) random record seeks |
| lower_bound | O(log N) | O(log N) random record seeks |
| range | O(log N + K) | search seeks + one sequential run |
| validate | Θ(N) | sequential scan |

These are logical operations. OS page cache, filesystem readahead and storage hardware affect physical I/O.
