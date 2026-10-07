# Implementation Notes

Build:
1. copy rows
2. qsort by primary key
3. reject duplicate primary keys
4. build secondary entries containing secondary key, primary key and primary-array row index
5. qsort secondary entries by (secondary, primary)

Secondary entries use row indexes instead of raw pointers so representation remains stable if the `DbIndex` object itself moves.

The implementation is immutable after build and intentionally does not model transactions, concurrent updates, MVCC or page splits.
