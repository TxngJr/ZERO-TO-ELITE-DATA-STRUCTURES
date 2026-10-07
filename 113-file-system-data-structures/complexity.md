# Complexity

Let I = inode capacity, E = directory entries, B = physical blocks, L = logical file blocks.

| Operation | Time |
|---|---:|
| create node | O(I + E) |
| directory lookup | O(E) |
| resize grow | O(B + added blocks) due free-block scan |
| resize shrink | O(released blocks) |
| logical block lookup | Θ(1) |
| validate | O(I + B + E²) |
| space | Θ(I + B + E) |

Production filesystems use indexed free-space and directory structures to improve these bounds.
