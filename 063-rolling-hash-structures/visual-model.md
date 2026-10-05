# Visual Model — Prefix Hash

Text bytes:

    a b c d e

prefix:

    H("")
    H("a")
    H("ab")
    H("abc")
    ...

Substring "bcd" = [1,4):

    prefix[4]
      - prefix[1] * B^3

computed independently under M1 and M2.
