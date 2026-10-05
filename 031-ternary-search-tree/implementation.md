# Implementation — ByteTST

Tree fields:
- root
- size
- empty_terminal

Node fields:
- symbol
- terminal
- low
- equal
- high

APIs:
- insert
- contains
- has_prefix
- count_prefix
- remove
- lexicographic visit
- validate

Keys use explicit byte pointer + length and can contain zero bytes.
