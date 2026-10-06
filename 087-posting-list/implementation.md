# Implementation

Dynamic sorted unique uint32_t array. Add uses lower_bound plus memmove. Intersection pre-reserves min(sizeA,sizeB). Gap codec uses base-128 continuation bytes and validates 32-bit overflow.
