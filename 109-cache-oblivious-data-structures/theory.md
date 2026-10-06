# Theory — Cache-Oblivious Layout

Cache-aware algorithms choose explicit block/tile sizes. Cache-oblivious algorithms/layouts seek locality at multiple scales without knowing exact cache parameters.

Morton/Z-order recursively divides a square into four quadrants and stores each quadrant contiguously in recursive order.

Benefits:
- nearby 2D coordinates tend to cluster physically
- locality exists at powers-of-two scales
- one layout can serve multiple cache levels

Costs:
- index mapping is more expensive than simple row-major arithmetic
- padding to a power-of-two square may waste space
- cache-oblivious is not a guarantee of universal speedup
