# Chapter 085 — Piece Table

Piece Table keeps the original file immutable and appends all inserted bytes to an add buffer. The current document is a sequence of pieces referencing ranges in either source.
Deletion edits only the piece sequence; bytes in original/add buffers remain immutable/append-only. Undo/redo systems can exploit this stable source history, though this chapter does not implement an undo log.
Insert/erase rebuild and coalesce piece descriptors transactionally. Consecutive pieces that reference contiguous ranges of the same source are merged.
This implementation is byte-oriented; Unicode/text semantics belong above the storage layer.
