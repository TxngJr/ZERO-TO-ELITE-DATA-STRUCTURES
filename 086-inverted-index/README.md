# Chapter 086 — Inverted Index

An inverted index maps each normalized term to the documents containing that term. It reverses the document→words relationship into term→posting-list lookup.
This implementation tokenizes ASCII alphanumeric runs, lowercases A-Z, stores exact term strings in a chained hash table, and keeps each term's document IDs sorted and unique.
Repeated occurrences of one term inside the same document do not increase document frequency. Positional frequencies are intentionally deferred.
AND query intersects two sorted document lists with a two-pointer scan.
