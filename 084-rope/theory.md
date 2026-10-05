# Theory — Rope

A rope's central idea is hierarchical sequence length metadata. Position queries descend by left-subtree weight instead of scanning from the beginning.
Split and concat/merge make editing compositional: insert = split + merge(prefix,new,suffix); erase = two splits + discard middle + merge survivors.
