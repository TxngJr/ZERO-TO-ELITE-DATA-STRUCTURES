# Theory — Ternary Search Tree

A TST has two dimensions of movement:

1. low/high: compare alternative symbols at the same key depth
2. equal: advance to the next key symbol

This is why it is neither an ordinary BST over whole strings nor an ordinary Trie.

## Ordering invariant

At any node x:

    all symbols in low subtree < x.symbol
    all symbols in high subtree > x.symbol

The equal subtree starts a new symbol-comparison level for the next input position.

## Lexicographic proof idea

Visiting low first emits all keys whose next byte is smaller.
Then current-symbol keys are emitted.
Then high emits larger next-byte keys.

Within equal subtree the same argument recurses on the following byte.
