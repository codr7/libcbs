#ifndef MACROS_H
#define MACROS_H

#include <stddef.h>
#include <stdint.h>

#define CALIGN(base, size) ({					\
      __auto_type _base = base;					\
      __auto_type _size = CMIN((size), _Alignof(max_align_t));	\
      (_base) + _size - ((ptrdiff_t)(_base)) % _size;		\
    })								\

#define CBASEOF(p, t, m) ({			\
      uint8_t *_p = (uint8_t *)p;		\
      _p ? ((t *)(_p - offsetof(t, m))) : NULL;	\
    })

#define _CID(x, y)				\
  x##y

#define CID(x, y)				\
  _CID(x, y)

#define CMAX(x, y) ({				\
      __auto_type _x = x;			\
      __auto_type _y = y;			\
      _x > _y ? _x : _y;			\
    })						\

#define CMIN(x, y) ({				\
      __auto_type _x = x;			\
      __auto_type _y = y;			\
      _x < _y ? _x : _y;			\
    })						\

#define CUNIQUE(x)				\
  CID(x, __COUNTER__)

#endif
