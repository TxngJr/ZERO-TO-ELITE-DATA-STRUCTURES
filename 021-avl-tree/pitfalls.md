# Pitfalls — AVL Tree

- mixing edge-height and node-height metadata
- updating heights in wrong order after rotation
- choosing LR/RL from wrong sign convention
- rebalancing insertion but not deletion
- assuming one deletion rotation always sufficient globally
- stale metadata despite correct BST ordering
- exposing node handles then copying successor key without identity contract
- recomputing full subtree heights and losing logarithmic updates
- forgetting duplicate policy
