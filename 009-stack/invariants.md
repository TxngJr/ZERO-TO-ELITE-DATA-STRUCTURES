# Invariants — Stack

## ArrayStack

- size <= capacity
- capacity==0 implies data==NULL in this implementation
- capacity>0 implies data!=NULL
- logical values occupy data[0..size)
- if size>0, top=data[size-1]

## LinkedStack

- size==0 iff top==NULL
- exactly size nodes reachable from top
- final next is NULL
- no cycle

## LIFO behavior

After successful push(x):
- size increases by one
- peek returns x
- previous sequence remains below x

After successful pop:
- returned value is previous top
- remaining sequence equals previous sequence without top

## Monotonic decreasing stack

From bottom to top:

    input[s0] >= input[s1] >= ... >= input[sk]

All stored indices are unresolved positions smaller than the current processing index.
