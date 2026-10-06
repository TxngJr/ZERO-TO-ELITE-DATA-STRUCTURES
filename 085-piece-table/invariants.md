# Invariants

1. every piece length > 0
2. piece ranges stay inside their source buffer
3. sum(piece.length)=logical size
4. original bytes never change
5. add_size only grows on successful insert
6. adjacent contiguous same-source pieces are coalesced
