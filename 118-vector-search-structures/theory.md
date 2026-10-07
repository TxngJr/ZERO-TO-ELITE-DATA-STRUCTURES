# Theory — Vector Search Structures

Vector search maps a query vector to nearby stored vectors under a distance/similarity metric.

Exact flat search is structurally simple:
- dense matrix of vectors
- optional per-vector metadata/norms
- bounded top-k selection structure

Approximate systems such as HNSW, IVF, PQ or tree indexes trade exactness for speed/memory. A correct exact index is therefore the baseline used to measure recall.

Top-k does not require sorting all N distances. A max-heap of size k keeps only current best candidates.
