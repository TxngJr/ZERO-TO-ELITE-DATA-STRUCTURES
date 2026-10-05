# Pitfalls — XOR Trie

- traversing LSB first for greedy XOR
- losing duplicate multiplicity
- decrementing before checking exact existence
- selecting child with subtree_count == 0
- mixing matched value and XOR score
- signed-shift/sign-bit ambiguity
- treating XOR threshold as <= instead of <
- publishing path before allocation capacity is secured
