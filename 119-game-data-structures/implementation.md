# Implementation Notes

`GameWorld` owns entity slots and one Position sparse set. A monotonically increasing world version changes on every structural/position mutation.

`SpatialGrid` has fixed bounds/cell size and stores:
- cell heads
- next link per entity slot
- source world version after rebuild

Grid rebuild requires all positions to fall inside configured bounds. Query rejects stale grids by comparing versions.

This chapter is intentionally single-threaded.
