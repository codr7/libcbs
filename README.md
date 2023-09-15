# intro
libcbs is a library of basic C utilities.

## lists
Lists are of the double linked, embedded kind.

```
  struct clist head;
  clist_init(&head);

  const int n = 10;
  struct list_item items[n];
  
  for (int i = 0; i < n; i++) {
    items[i].value = i;
    clist_push_back(&head, &items[i].list);
  }

  int i = 0;
  
  CLIST_DO(&head, il) {
    assert(CBASEOF(il, struct list_item, list)->value == i++);
  }
```

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
