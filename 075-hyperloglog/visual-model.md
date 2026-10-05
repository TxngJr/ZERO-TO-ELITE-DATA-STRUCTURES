# Visual Model — HyperLogLog

64-bit hash = [p index bits][remaining bits]
index selects register j.
rho(remaining) updates M[j] = max(M[j],rho).
