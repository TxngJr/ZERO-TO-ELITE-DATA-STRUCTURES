# Complexity

offset O(ndim). permute O(ndim). slice O(1) metadata after validation. No data movement occurs in these view operations. Physical traversal performance still depends heavily on stride/locality.
