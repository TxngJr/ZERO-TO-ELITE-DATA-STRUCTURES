# Pitfalls — D-ary Heap

- accepting d=0 or d=1
- using Binary Heap child formulas
- overflowing d*i+1
- scanning d children past size
- claiming pop O(log_d n) while ignoring d-way child scan
- assuming larger d always faster
- forgetting build remains bottom-up
