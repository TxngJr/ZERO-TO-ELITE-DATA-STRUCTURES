# Invariants — IntBPlusTree

1. logical keys exist only in leaves
2. leaf keys strictly increasing
3. internal key[i] = minimum key of child[i+1]
4. internal node has key_count+1 children
5. non-root nodes satisfy minimum occupancy
6. all leaves have same depth
7. leaf chain follows exact left-to-right leaf order
8. leaf chain keys are globally strictly increasing
9. total leaf key count equals tree size
10. unique logical keys
