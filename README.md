# intro
libcbs is a library of basic C utilities.

## deques
Deques allow efficient pushing and popping to both ends by allocating memory in linked blocks of user defined size. Compared to vectors; random access is slightly slower and memory locality worse due to following pointers at block boundaries; on the other hand, pointers to values are stable while pushing.