# Implementation — Reservoir Sampling

Reservoir stores capacity, current sample_count, seen count, RNG state and uint64_t sample items.
uniform_bounded uses rejection threshold before modulo.
seen overflow is rejected before mutation.
