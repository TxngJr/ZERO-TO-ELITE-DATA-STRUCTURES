# Implementation Notes

Process slots contain generation, priority, state and one intrusive `next_ready` index.

A separate free-slot stack makes spawn O(1). Ready queues are singly linked through process-table slots.

Termination of an arbitrary READY process scans its queue because this teaching implementation stores only next links. A production scheduler may use doubly linked intrusive nodes, trees, heaps or specialized run-queue structures.

The validator cross-checks:
- active/free partition
- queue membership
- queue head/tail/count
- state consistency
- running slot uniqueness
- free-stack validity
