# Complexity

Let α be interner hash load factor, S active symbols, U uses of one definition.

| Operation | Average Time |
|---|---:|
| intern/find name | O(1) expected |
| declare symbol | O(1) expected |
| lookup symbol | O(1) expected |
| enter scope | Θ(1) |
| leave scope | Θ(symbols declared in that scope) |
| add def-use edge | Θ(1) |
| collect users | Θ(U) |
| validate symbol tables | O(capacities) |
| validate def-use graph | O(values + edges) |

Worst-case open-addressing lookup can degrade with pathological clustering.
