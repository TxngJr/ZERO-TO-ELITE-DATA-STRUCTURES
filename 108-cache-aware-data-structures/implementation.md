# Implementation Notes

Offset:

```text
tile_r = row / tile_rows
tile_c = col / tile_cols
in_r   = row % tile_rows
in_c   = col % tile_cols

tile_index = tile_r * tile_col_count + tile_c
offset = tile_index * tile_elems + in_r * tile_cols + in_c
```

Creation เช็ก overflow ของ:
- tile rows × tile cols
- tile counts
- total padded elements
- bytes for `double`

`cam_validate` คำนวณ metadata ซ้ำและตรวจ padding cells ของ edge tiles ต้องเป็น zero.
