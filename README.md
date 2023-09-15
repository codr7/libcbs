# intro
libcbs is a library of basic C utilities.

## lists
Lists are of the double linked, embedded kind.

```
  #include <cbs/list.h>

  struct list_item {
    struct clist list;
    int value;
  };

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
Deques are value based and allow efficient pushing and popping to both ends by allocating memory in linked blocks. Compared to vectors; random access is slightly slower and memory locality worse due to following pointers at block boundaries; on the other hand, pointers to values are stable while pushing.

```
  #include <cbs/deque.h>
  
  struct cdeque d;
  cdeque_init(&d, 2, sizeof(int));
  const int n = 20;

  for (int i = n/2-1; i >= 0; i--) { *(int *)cdeque_push_front(&d) = i; }
  assert(d.length == n/2);

  for (int i = n/2; i < n; i++) { *(int *)cdeque_push_back(&d) = i; }
  assert(d.length == n);

  for (int i = 0; i < n; i++) { assert(*(int *)cdeque_get(&d, i) == i); }

  cdeque_deinit(&d);
```

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
