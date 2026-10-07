# Visual Model

```text
External node ID
      |
      v
Hash index -> dense node index
                  |
          +-------+-------+
          |               |
      out_offsets      in_offsets
          |               |
      out_edge_ids     in_edge_ids

label_order:
(label,id,node_index)
(label,id,node_index)
...
```

Mutation increments graph version.
Queries require:

```text
index_version == graph_version
```
