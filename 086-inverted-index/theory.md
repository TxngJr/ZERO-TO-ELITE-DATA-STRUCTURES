# Theory — Inverted Index

Forward view: document -> terms. Inverted view: term -> documents. Search engines add richer payloads such as term frequency, positions, field IDs and scores.
Sorted unique doc IDs make Boolean intersection efficient and prepare the way for posting-list compression.
