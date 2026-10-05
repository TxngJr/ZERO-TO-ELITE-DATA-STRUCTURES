# Pitfalls — Priority Queue

- assuming heap array is fully sorted
- wrong parent formula for index 0
- choosing smaller child during max-heap sift-down
- forgetting to compare both children
- off-by-one after size-- in pop
- promising stable equal-priority order without metadata/contract
- ignoring dynamic-array growth failure
- confusing max priority with min priority convention
- using heap when arbitrary ordered iteration is the real requirement
- assuming priority policy guarantees fairness
- decrease-key without a way to locate arbitrary item
- child-index arithmetic overflow in careless implementations
