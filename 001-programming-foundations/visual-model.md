# Visual Model — Programming Foundations

## Name, object, value and address

    source name
       x
       |
       v
    +---------+
    | object  |  address: &x
    | int     |
    | value=5 |
    +---------+

Pointer:

    p
    +----------------+
    | address of x   | --------+
    +----------------+         |
                               v
                            +-----+
                            |  5  |
                            +-----+

Dereference *p means access the pointee object.

## Function call

    caller
      |
      | arguments
      v
    function
      |
      | return value / state effects
      v
    caller continues

Pass-by-value creates a parameter object/value according to language semantics.
Pointer/reference-based APIs may let the callee reach caller-owned state.

## Recursive call chain

    sum(3)
      -> 3 + sum(2)
              -> 2 + sum(1)
                      -> 1 + sum(0)
                              -> 0

Calls return in reverse nesting order.

## Struct

    Point
    +---------+
    | x       |
    +---------+
    | y       |
    +---------+

Exact byte offsets/padding are ABI/compiler-dependent and will be revisited later.
