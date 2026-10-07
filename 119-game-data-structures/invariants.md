# Invariants

1. Active/free slots partition entity capacity.
2. Every active entity has nonzero generation.
3. sparse[slot] is NONE or points to matching dense slot.
4. Every dense position slot is active.
5. Dense position slots are unique.
6. position_count <= active_count.
7. Grid query is valid only when built_version == world_version.
8. Every positioned entity appears in exactly one spatial cell after rebuild.
9. Grid cell membership matches position-to-cell mapping.
10. No grid chain contains duplicate entity slots.
