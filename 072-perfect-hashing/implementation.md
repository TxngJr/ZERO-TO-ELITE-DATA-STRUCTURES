# Implementation — Perfect Hashing

Input is sorted/deduplicated first.
Top seed retries until quadratic bucket-size sum <=4n.
Each bucket allocates s^2 slots and retries a seed until exact collision-free placement.
