# Chapter 042 — Sparse vs Dense Graph Representation

## Goal

Chapter 039 เปรียบเทียบ Edge List / Matrix / Adjacency List แบบ static.

บทนี้ไปอีกขั้น: สร้าง **AdaptiveGraph** ที่เลือก representation ตาม density และสามารถสลับ:

    Adjacency List <-> Adjacency Matrix

โดยรักษา logical graph เดิม.

นี่เป็น teaching model ของ workload-aware representation selection ไม่ใช่ universal production heuristic.

## Density

For simple directed graph without self-loops:

    E_max = V(V-1)

Undirected:

    E_max = V(V-1)/2

Density percent:

    100 * E / E_max

Sparse:
density ต่ำ

Dense:
density สูง

แต่ไม่มี threshold เดียวที่ดีที่สุดทุก workload.

## Why Adjacency List for Sparse Graphs?

Space:

    Theta(V+E)

Neighbor traversal:

    Theta(deg(v))

When E << V^2:
- avoids huge matrix
- traversal touches only existing arcs
- memory footprint usually much lower

## Why Matrix for Dense Graphs?

Space:

    Theta(V^2)

But when E is already Theta(V^2), that cost is no longer disproportionately wasteful.

Advantages:
- edge lookup Theta(1)
- predictable contiguous indexing
- potential bitset/SIMD optimization
- dense algorithms often naturally matrix-oriented

## Adaptive Policy

Teaching policy has two thresholds:

    promote_percent
    demote_percent

Require:

    demote_percent < promote_percent

Start:
    Adjacency List

If density >= promote:
    attempt conversion to Matrix

If currently Matrix and density <= demote:
    attempt conversion to Adjacency List

## Why Two Thresholds?

Suppose one threshold = 20%.

If density changes:
    19.9 -> 20.1 -> 19.9 -> 20.1

a one-threshold design can repeatedly convert representations.

This is **thrashing**.

Hysteresis example:

    promote at 25%
    demote at 10%

Once promoted, graph remains Matrix through the middle band.
Once demoted, graph remains List through the middle band.

## Conversion Must Preserve Semantics

Conversion creates a new backend first.

Then replay every logical edge:
- directed: replay every outgoing arc
- undirected: replay only when u < v to avoid double-counting physical symmetric arcs

Only after successful replay:

    swap backend
    free old backend
    switch_count++

If allocation/replay fails:
- keep old backend
- logical mutation that triggered the attempt remains valid
- optimization failure must not corrupt graph semantics

## Conversion Complexity

Adj List -> Matrix:
- allocate V^2 cells
- enumerate existing edges
- conceptually O(V^2 + E)

Matrix -> Adj List:
- matrix neighbor enumeration scans V cells per vertex
- O(V^2)

Representation switching is expensive.

Therefore adaptive conversion only makes sense if future workload benefits enough to amortize conversion cost.

## Workload Matters Beyond Density

Two graphs with equal density can prefer different representations.

Workload A:
- millions of has_edge queries
- few traversals

Matrix may win earlier.

Workload B:
- repeated neighbor scans
- moderate degree
- few edge-existence queries

Adjacency List may remain better.

Other factors:
- weight size
- bitset matrix availability
- cache hierarchy
- mutation rate
- allocator overhead
- vertex ID compactness
- GPU/vectorized processing

## Vertex-ID Space Matters

Matrix cost depends on number of possible vertex IDs represented.

If IDs are sparse huge integers, remapping to compact 0..V-1 IDs may be essential before considering a matrix.

This course graphs already use compact IDs.

## Memory Crossover

Teaching Matrix from Chapter 039 stores roughly per cell:
- 1 byte presence
- sizeof(int) weight

Adjacency List stores vector headers + neighbor entries + spare capacity.

Exact crossover depends on:
- struct padding
- vector growth
- allocator metadata
- weight representation

Therefore the benchmark reports actual teaching allocation estimates rather than claiming a universal algebraic cutoff.

## AdaptiveGraph API

Supports:
- add/remove edge
- has_edge
- neighbor iteration
- current backend kind
- density percent
- allocated-byte estimate
- switch count
- backend validation

The logical API remains stable while physical representation changes.

## Complexity

When backend is Adj List:
- has_edge O(log degree)
- neighbor scan Theta(degree)

When backend is Matrix:
- has_edge Theta(1)
- neighbor scan Theta(V)

Switch:
- up to Theta(V^2 + E)

Storage:
- current backend dependent

This means an individual add/remove that triggers conversion can be much more expensive than ordinary mutation.

## When Not to Adapt Automatically

Avoid automatic switching when:
- latency spikes from conversion are unacceptable
- graph is extremely large
- density oscillates despite hysteresis
- workload phase is short
- conversion doubles peak memory temporarily
- representation choice is externally fixed by algorithm/storage format

Static choice can be better.

## Files

- src/adaptive_graph.*
- tests/test_adaptive_graph.c
- examples/adaptive_demo.c
- benchmarks/adaptive_density_benchmark.c
- theory/visual-model/implementation/complexity/invariants/pitfalls
- lab/exercises/quiz/references
