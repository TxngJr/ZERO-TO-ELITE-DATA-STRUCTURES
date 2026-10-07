# Complexity

Let C = flow-table capacity.

| Operation | Expected | Worst |
|---|---:|---:|
| IPv4 route add | O(prefix length) | O(32) |
| IPv4 LPM lookup | O(32) | O(32) |
| route remove | O(prefix length) | O(32) |
| flow put/get/remove | O(1) | O(C) |
| flow rehash | Θ(C) | Θ(C) |
| route validate | Θ(nodes) | Θ(nodes) |
| flow validate | O(C + probe work) | O(C²) pathological clustering |

IPv4 bit width is fixed, so trie lookup is constant with respect to route count.
