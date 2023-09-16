# intro
libcbs is a library of basic C utilities.

## dependencies
The only dependency is a C2X compiler and standard library.

## setup
The following script builds both static and dynamic versions of the library and runs the test suite.

```
git clone https://github.com/codr7/libcbs.git
cd libcbs
mkdir build
cd build
make
./test
```

## lists
Lists are double linked and embedded. Think of them as the easiest and cheapest way to string together a sequence of values. Since the links are embedded in the items, it's easy to get good memory locality and avoid extra allocations. And because they are double linked, deletion is O(1) and only requires access to the item. One limitation to keep in mind is that you need to embed separate links for every list that an item is going to be a member of simultaneously.

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
Deques are value based and allow efficient pushing and popping to both ends by allocating memory in linked blocks. Compared to vectors; random access is slightly slower and memory locality worse due to pointer traversal at block boundaries; on the other hand, pointers to values are stable while pushing.

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
