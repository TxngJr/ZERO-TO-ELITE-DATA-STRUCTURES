# Implementation

`ConcurrentIntSet` uses a sorted unique dynamic array + one `pthread_mutex_t`.

The lock covers binary search and every mutation including `realloc`, pointer publication, memmove and metadata update. Readers also lock because reading an array while another thread reallocates/moves it would race.

Snapshot copies under the lock, giving one consistent point-in-time state but increasing lock hold time.

Grow uses a temporary realloc pointer: allocation failure leaves old storage/state valid.
