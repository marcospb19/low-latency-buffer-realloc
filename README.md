# low-latency-buffer-realloc
Reallocation of growable buffers with lower worst case latency, this project is a scratch of an concept yet to be proven (if I abandoned this, I'm sorry).

I need to study more about alocators before assuming that I'm 100% right.

There are three possible outcomes out of this, if I give it the research time needed:

1. Data structures with **true** O(1) grow, instead of amortized O(1) (LATENCY IMPROVEMENTS!).
2. Previous point plus the creation of new allocator that performs perfectly in non-segmented heap memory chunks.
3. Concept failure.
