# Theory — Reservoir Sampling

The key invariant is uniform inclusion probability over stream positions.
Induction shows an old item survives step t with probability ((t-1)/t), balancing the new item's k/t replacement probability.
Sampling quality depends on RNG quality and unbiased bounded-integer generation.
