# Chapter 083 — Gap Buffer

Gap Buffer stores text in one array with a contiguous unused gap positioned near the edit cursor.
Insert at the gap consumes free space; delete expands the gap; moving the edit position moves bytes across the gap. This makes repeated local edits cheap while distant cursor moves can copy O(distance) bytes.
The implementation is byte-oriented, not Unicode-code-point aware. UTF-8 editing would need a higher-level boundary policy.
Geometric growth creates a larger gap while preserving prefix/suffix text.
