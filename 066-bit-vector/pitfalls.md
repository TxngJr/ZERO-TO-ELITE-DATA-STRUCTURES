# Pitfalls — Bit Vector

- defining rank as inclusive in one place and exclusive elsewhere
- mixing 0-based/1-based select
- selecting padding zeros in final word
- binary-searching wrong prefix inequality
- shifting by 64 when building a prefix mask
- allowing mutation without rebuilding directory
- claiming this full directory is succinct
