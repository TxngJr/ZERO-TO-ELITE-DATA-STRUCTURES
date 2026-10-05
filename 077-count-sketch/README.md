# Chapter 077 — Count Sketch

Count Sketch estimates signed stream frequencies using two hash choices per row: one chooses a bucket, another chooses a sign (+1 or -1).

Update key x by delta:
    counter[row][h_row(x)] += s_row(x) * delta

Query each row by multiplying the counter back by the same sign, then take the median across rows.

Unlike Count-Min Sketch, collision noise can be positive or negative. The estimator is therefore not one-sided and is useful in turnstile/signed-update settings under appropriate assumptions.

This implementation requires odd depth so median is unambiguous, rejects INT64_MIN delta to avoid nonrepresentable negation, prechecks every touched counter before mutation, and rejects query states where sign reversal of INT64_MIN would overflow.

Complexity: update/query O(depth), merge O(width*depth), storage O(width*depth).
