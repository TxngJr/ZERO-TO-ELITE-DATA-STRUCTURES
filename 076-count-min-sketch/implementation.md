# Implementation — Count-Min Sketch

Flat uint64_t counter matrix, one deterministic seed per row.
Update checks all rows for overflow before applying delta.
Validator confirms every row sum equals total_weight.
