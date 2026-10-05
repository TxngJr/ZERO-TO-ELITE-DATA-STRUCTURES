# Invariants — ByteString

For every valid ByteString:

1. length <= capacity
2. data != NULL
3. allocation has at least capacity+1 bytes
4. data[length] == '\0'
5. logical bytes are data[0..length)
6. no arithmetic overflow occurred in requested new length/allocation size

## Append preservation

Need new_length=old_length+count without overflow.
reserve establishes capacity>=new_length.
copy bytes to old_length...
set length=new_length.
set terminator.

## Insert preservation

index<=length.
After reserve, shift suffix including old terminator enough room to the right.
Copy inserted bytes.
Update length.
Terminator remains/restored at new length.

## Erase preservation

Clamp requested count to available suffix.
Move bytes after erased range left, including terminator.
Decrease length.
