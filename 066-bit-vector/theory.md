# Theory — Rank and Select

Rank and select are dual navigation primitives over bit sequences.

rank tells:
    how many target bits appear before a position

select tells:
    where the kth target bit occurs

They power:
- succinct trees
- compressed indexes
- wavelet trees/matrices
- FM-index components
- compressed graph representations

This chapter uses a simple full per-word prefix directory before later studying succinct directories.
