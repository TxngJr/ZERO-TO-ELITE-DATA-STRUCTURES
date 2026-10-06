# Theory — Piece Table

A piece is (source,start,length). Logical text is the concatenation of pieces, not either backing buffer. Editing changes references rather than rewriting the whole original text.
The add buffer grows monotonically, so deleted inserted bytes may remain unreachable but still occupy backing storage.
