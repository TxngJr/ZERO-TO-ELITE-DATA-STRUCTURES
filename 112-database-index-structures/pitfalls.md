# Common Mistakes

- assume secondary key is unique.
- sort secondary only by secondary key and rely on unstable qsort order.
- store stale raw pointers into an array that may move.
- use primary binary search on unsorted input.
- forget to reject duplicate primary keys.
- call a sorted in-memory index a transactional database index.
- ignore write amplification and maintenance cost of extra indexes.
