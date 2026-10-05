# Implementation — ByteAhoCorasick

State:
- sparse byte transitions
- failure link
- output link
- direct terminal output records

Output record:
- pattern ID
- pattern length

Build:
1. insert all patterns into trie
2. BFS states
3. compute failure links
4. compute nearest terminal output links

Match:
- one pass over text
- failure fallback
- direct + output-link emissions
