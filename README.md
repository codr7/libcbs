# intro
libcbs is a library of basic C utilities.

## deques
Deques allow efficient pushing and popping to both ends by allocating memory in linked blocks of user defined size. Compared to vectors; random access is slightly slower and memory locality worse due to following pointers at block boundaries; on the other hand, pointers to values are stable while pushing.

## structured logging

```
  #include <cbs/slog.h>
  
  struct cslog s;
  cslog_init(&s, stdout, CSLOG_TEXT);
  CSLOG(&s, "hello slog", cslog_int("int", 42));
  cslog_deinit(&s);
```

Output:
```
timestamp=2023-09-15 20:28:21, message="hello slog", int=42
```
