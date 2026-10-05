# Invariants — Perfect Hashing

1. input set is deduplicated
2. top bucket count equals unique n
3. bucket with s keys has s^2 slots
4. no two stored keys share one secondary slot
5. exact key comparison prevents false positives
6. total secondary slots <=4n
