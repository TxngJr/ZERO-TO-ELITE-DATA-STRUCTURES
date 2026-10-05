# Theory — Rolling Hash

Prefix polynomial hashes turn substring comparison into arithmetic on precomputed prefixes and powers.

The central engineering lesson is:

    hash mismatch => proof of inequality
    hash match    => candidate equality

For correctness-critical code, verify candidates.

Rolling hash is powerful when most candidates fail quickly.
