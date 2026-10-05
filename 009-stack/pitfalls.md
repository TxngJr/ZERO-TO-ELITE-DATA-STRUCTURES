# Pitfalls — Stack

- confusing size with top index
- reading data[size] instead of data[size-1]
- underflow on empty pop
- linked push losing old top
- linked pop freeing before reading next/value
- geometric growth overflow
- assuming linked is always faster because push is Θ(1)
- confusing Stack ADT with runtime call stack
- using wrong comparison for duplicate values in monotonic stack
- concluding nested while implies Θ(n²) without aggregate analysis
- storing only values when answers require original indices
- not initializing unresolved answers
