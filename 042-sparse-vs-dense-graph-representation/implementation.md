# Implementation — AdaptiveGraph

Fields:
- vertex_count
- directed
- promote_percent
- demote_percent
- GraphRepr *backend
- switch_count

Initial backend:
    GRAPH_REPR_ADJ_LIST

After successful add/remove:
    maybe_switch()

Conversion is transactional:
1. create target backend
2. replay logical edges
3. validate new backend
4. swap pointer
5. free old backend

Failed conversion leaves the original backend active.
