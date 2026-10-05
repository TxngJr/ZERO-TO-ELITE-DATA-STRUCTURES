# Theory — Suffix Array

Suffix Array stores sorted suffix positions rather than materializing suffix strings.

Prefix doubling works because if rank classes correctly represent prefixes of length span, then sorting pairs of those classes correctly orders prefixes of length 2*span.

Kasai exploits:

    LCP(suffix i+1, suffix j+1)
    >= LCP(suffix i, suffix j)-1

so the matching pointer does not restart from zero for every suffix.
