#ifndef ASSERT_HPP
#define ASSERT_HPP

#include <stdio.h>

#define ASSERT_WARNING \
  printf ("(%s) Warning: Assertion failed at line %i.\n", __FUNCTION__, __LINE__); \

#define ASSERT(expr) \
  if (!(expr)) { \
    ASSERT_WARNING \
  } \

#define ASSERT_RETURN(expr) \
  if (!(expr)) { \
    ASSERT_WARNING \
    return; \
  } \

#define ASSERT_RETURN_VAL(expr, val) \
  if (!(expr)) { \
    ASSERT_WARNING \
    return val; \
  } \

#endif // ASSERT_HPP