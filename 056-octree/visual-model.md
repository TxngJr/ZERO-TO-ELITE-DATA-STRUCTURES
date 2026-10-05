# Visual Model — Octree

A cube is split by three planes:

    x = mid_x
    y = mid_y
    z = mid_z

yielding 8 octants.

Index bits:

    bit 0 -> east
    bit 1 -> north
    bit 2 -> upper

Example:
    east + south + upper
    = 1 + 0 + 4
    = octant 5
