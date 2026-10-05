# Visual Model — Count Sketch

row r: bucket = h_r(x), sign = s_r(x) in {-1,+1}.
update adds sign*delta.
query row estimate = sign*counter, final estimate = median.
