# Visual Model — ADT

## Abstraction boundary

    caller
      |
      | public operations
      v
    +------------------------+
    |       Stack ADT        |
    | push/pop/peek/size     |
    +------------------------+
               |
               | implemented by
               v
    +------------------------+
    | concrete representation|
    | data,size,capacity     |
    +------------------------+

Caller depends on behavior, not fields.

## Concrete to abstract

Concrete:
    data=[10,20,30,_,_]
    size=3
    capacity=5

Representation function:

    R(concrete) = Stack[10,20,30]
                         bottom -> top

capacity is implementation metadata, not part of the abstract stack value.

## Operation

    valid state
       |
       | push(40)
       v
    internal transition
       |
       | invariant restored
       v
    valid state representing [10,20,30,40]
