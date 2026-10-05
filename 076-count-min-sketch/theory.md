# Theory — Count-Min Sketch

CMS is a one-sided estimator for nonnegative streams: hash collisions can only increase counters.
Taking the minimum across independent-ish rows reduces collision contamination.
Negative updates break the simple no-underestimate property unless a different model/design is used.
