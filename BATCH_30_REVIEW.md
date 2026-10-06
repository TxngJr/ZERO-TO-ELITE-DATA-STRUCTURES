# Batch 30 — Negative Review and Audit

## Scope

- 088 Sparse Matrix Representations
- 089 Matrix/Tensor Storage Layout
- 090 Compressed Data Structures

## Chapter 088 Review

Checked:
- COO dimensions must be nonzero
- COO coordinates are bounds-checked
- explicit zero additions are ignored
- conversion copies input and does not mutate COO ordering
- duplicate coordinates are sorted together and summed
- duplicate groups whose final sum is zero are omitted
- CSR row_ptr length is rows+1 and terminates at nnz
- CSC col_ptr length is cols+1 and terminates at nnz
- canonical CSR columns strictly increase within every row
- canonical CSC rows strictly increase within every column
- CSR/CSC point lookup uses binary search inside the relevant segment
- SpMV checks vector dimensions and touches only stored entries
- randomized test compares every sparse cell and SpMV output to dense reference
- first CI build caught a missing <stdint.h> include for SIZE_MAX; the source was patched before test execution

Complexity:
- COO append amortized O(1)
- conversion O(k log k)
- CSR get O(log nnz_row)
- CSC get O(log nnz_col)
- CSR SpMV O(rows+nnz)

## Chapter 089 Review

Checked:
- layout dimensions restricted to 1..8
- every logical dimension has nonzero shape and stride
- row-major construction sets last-axis stride to 1
- column-major construction sets first-axis stride to 1
- stride multiplication, total size and offset arithmetic are overflow-checked
- permutation rejects duplicate/missing axis IDs
- positive-step slice validates final selected coordinate
- slice updates base offset and multiplies the sliced dimension stride
- views do not own or move storage
- strides are explicitly measured in elements, not bytes
- audit corrected the stepped-slice test expected physical offset from 54 to 59 before commit

Complexity:
- offset/permute O(ndim)
- slice metadata O(1) after fixed-size validation
- traversal cost depends on physical stride/locality rather than metadata complexity

## Chapter 090 Review

Checked:
- input sequence must be strictly increasing
- block_size must be positive
- block count uses overflow-safe ceil division precondition
- first value of each block is stored as an absolute checkpoint
- only positive within-block gaps are varint encoded
- offsets delimit each block payload and end exactly at byte_count
- 64-bit varint decoder permits at most ten bytes and validates the tenth byte width
- cumulative gap addition checks UINT64 overflow
- get decodes only within one block
- lower_bound binary-searches block bases then scans at most one block
- validator decodes every block and verifies cross-block strict increase
- encoded_bytes intentionally reports delta payload only; base/offset metadata is documented separately so payload ratio is not misrepresented as total memory usage

Complexity:
- build/validate O(n)
- get O(block_size)
- lower_bound O(log blocks + block_size)
- metadata O(blocks)

## Testing Review

- Sparse Matrix: dense differential check over every cell plus SpMV
- Tensor Layout: contiguous and non-contiguous view offset checks
- Compressed Sequence: 100k exact get checks sampled every 97 positions and broad lower_bound differential sweep
- CI retains warnings, ASan/UBSan and 60-second per-test watchdog

## Definition of Done

Batch 30 is complete only when Fedora CI configures/builds Chapters 001–090 and the full CTest suite passes with sanitizers enabled.
