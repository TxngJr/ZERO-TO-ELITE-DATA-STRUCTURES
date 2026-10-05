# Pitfalls — B-Tree

- splitting at wrong median
- moving wrong number of children
- descending into minimum-occupancy child without repair
- borrow shifts off by one
- merge forgets parent separator
- root not shrunk after delete
- validating only local sortedness but not global ranges
- confusing B-Tree order terminology across textbooks
