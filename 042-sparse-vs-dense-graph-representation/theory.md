# Theory — Sparse vs Dense

## Density is one signal

Density summarizes E relative to maximum possible edges, but ignores operation mix.

Representation selection is a multidimensional optimization:
- memory
- lookup frequency
- neighbor scans
- updates
- conversion cost
- cache behavior

## Hysteresis

Two thresholds create a stable band.

This is the same systems idea used in:
- autoscaling
- cache policies
- control systems
- storage compaction triggers

The goal is to prevent rapid state oscillation near a boundary.

## Semantic-preserving conversion

A representation conversion is correct if every logical edge and weight is identical before and after conversion.

Physical entry count is allowed to change.
