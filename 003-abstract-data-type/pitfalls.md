# Pitfalls — ADT

- treating a class/struct syntax as automatically being an ADT
- exposing mutable fields that let callers break invariants
- writing tests against capacity/layout rather than behavior
- changing behavior silently while calling it "implementation refactoring"
- failing to specify error behavior for empty pop/allocation failure
- mixing ownership of internal storage with caller ownership
- incrementing size before a fallible growth step succeeds
- assuming opaque type means zero runtime cost or perfect abstraction
- adding operations that violate the intended abstraction just because they are easy to implement
