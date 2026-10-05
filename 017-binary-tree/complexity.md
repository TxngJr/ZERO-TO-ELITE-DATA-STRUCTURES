# Complexity — Binary Tree

Let n=tree size, h=height, s=removed subtree size.

| Operation | Time |
|---|---:|
| root/value/left/right accessor | Theta(1) |
| membership by parent chain | O(h) |
| add left/right | O(h) in this defensive API |
| set value | O(h) due membership check |
| height | Theta(n) |
| validate | Theta(n) valid tree |
| remove subtree | O(h+s) |
| destroy | Theta(n) |

Space:
- node storage Theta(n)
- recursive height/free/validate call depth O(h)

If membership were guaranteed by stronger type/arena design, child insertion itself can be O(1).
