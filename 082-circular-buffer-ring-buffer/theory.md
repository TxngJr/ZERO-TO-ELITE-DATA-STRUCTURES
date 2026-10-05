# Theory — Ring Buffer

The array has no moving suffix on ordinary queue operations. Advancing head and computing wrapped indices replaces O(n) shifting with O(1) metadata updates.
Full/empty must be disambiguated with size or a reserved slot; this implementation stores size explicitly.
