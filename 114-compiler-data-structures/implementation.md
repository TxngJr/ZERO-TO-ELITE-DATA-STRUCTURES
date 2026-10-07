# Implementation Notes

`CompilerTables` uses fixed maximum capacities supplied at creation:
- interned names
- active symbols
- scope depth

Hash table capacity is a power of two at least about 2× name capacity to keep load factor <= 0.5.

Each symbol stores:
- NameId
- kind
- type tag
- scope depth
- previous binding index for the same name

`UseDefGraph` uses:
- head index per definition
- flat edge arena
- next index per edge

Both structures are single-threaded teaching implementations.
