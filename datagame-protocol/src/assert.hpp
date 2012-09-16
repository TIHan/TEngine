#ifndef ASSERT_HPP
#define ASSERT_HPP

#include <stdio.h>

#define ASSERT_WARNING \
  printf ("(%s) Warning: Assertion failed at line %i.\n", __FUNCTION__, __LINE__); \

#define assertReturn(expr) \
  if (!(expr)) { \
    ASSERT_WARNING \
    return; \
  } \

#define assertReturnVal(expr, val) \
  if (!(expr)) { \
    ASSERT_WARNING \
    return val; \
  } \

#endif // ASSERT_HPP