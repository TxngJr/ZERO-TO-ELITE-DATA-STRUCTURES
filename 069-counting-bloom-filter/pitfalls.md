# Pitfalls — Counting Bloom Filter

- assuming deletion proves exact membership
- removing a false-positive key
- saturating counters then decrementing as if exact
- ignoring duplicate hash positions
- failing to rollback partial mutation
- using counters much wider than workload needs without memory analysis
- assuming removed key must immediately query negative because collisions can keep counters positive
