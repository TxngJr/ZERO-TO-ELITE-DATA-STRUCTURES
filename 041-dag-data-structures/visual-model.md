# Visual Model — Dependency DAG

    compile-A ----\
                   > link ---> package
    compile-B ----/      \
                          -> tests

indegree:
    compile-A = 0
    compile-B = 0
    link      = 2

Initial ready queue:
    compile-A, compile-B

After both complete:
    link becomes ready.
