# Pitfalls — Hash Table

- resizing but not rehashing entries
- incrementing size when updating existing key
- comparing only hash instead of key
- forgetting remove to decrement size
- losing chain suffix during insertion/removal
- freeing nodes during resize accidentally
- assuming iteration order stable
- claiming worst-case O(1)
- ignoring hash/equality cost for complex keys
- growing after mutation in a way that leaves invalid state on allocation failure
- not checking bucket-count overflow
