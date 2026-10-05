# Theory — HyperLogLog

Rare long runs of leading zeros contain information about how many independent hashes were observed.
Many registers reduce variance by combining independent-ish bucket observations.
HLL is duplicate insensitive because repeating a hash cannot increase a register beyond its existing max.
