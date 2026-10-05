# Implementation — HyperLogLog

Hash uint64_t with mixed fixed seed.
Use high p bits for register index.
Use clz on shifted remainder, with explicit all-zero remainder case.
Merge uses register-wise max.
