# Chapter 075 — HyperLogLog

HyperLogLog (HLL) estimates distinct cardinality using a fixed array of small registers rather than storing distinct keys.

Precision p chooses:
    m = 2^p registers

The top p hash bits select a register. The remaining bits determine rho: the position of the first 1 bit, represented here as leading-zero count + 1. Each register stores the maximum observed rho.

Estimator uses the harmonic mean-like quantity:
    E = alpha_m * m^2 / sum(2^-M[j])

For small cardinalities with empty registers, this implementation uses linear-counting correction:
    E = m * ln(m/V)
where V is zero-register count.

For very large estimates it applies the classic 64-bit large-range correction when applicable.

Merge of equal-precision HLLs is register-wise maximum, which makes HLL useful for distributed aggregation.

Precision trade-off: more registers consume more memory and reduce standard error. Classical HLL relative standard error is approximately 1.04/sqrt(m), under suitable uniform hashing assumptions.

This implementation stores one byte per register for clarity; packed 5/6-bit register encodings can reduce memory further.

Supported precision p is 4..18.
