#ifndef ASSERT_HPP
#define ASSERT_HPP

#include <stdio.h>

#define assertReturn(expr) \
  if (!(expr)) { \
    printf ("(%s) Warning: Assertion failed at line %i.\n", __FUNCTION__, __LINE__); \
    return; \
  } \

#define assertReturnVal(expr, val) \
  if (!(expr)) { \
    printf ("(%s) Warning: Assertion failed at line %i.\n", __FUNCTION__, __LINE__); \
    return val; \
  } \

#endif // ASSERT_HPP