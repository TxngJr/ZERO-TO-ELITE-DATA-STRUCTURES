# Theory — Gap Buffer

Logical text is prefix + suffix; the physical gap is not part of the document. Locality of editing matters more than a single worst-case operation: typing repeatedly at one cursor can be near O(1) amortized per byte.
