# Implementation

TensorLayout supports up to 8 dimensions and stores strides in elements, not bytes. All offset, stride and total-element calculations include size_t overflow checks.
