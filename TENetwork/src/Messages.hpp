#ifndef __MESSAGES_HPP_
#define __MESSAGES_HPP_

#include <stdio.h>

#define MAX_ERROR_LEN 256

/* ASSERT */

#define ASSERT_MESSAGE(str) \
  printf ("(%s) Line %i Assertion: %s\n", __FUNCTION__, __LINE__, str); \

#define ASSERT(expr, str) \
  if (!(expr)) { \
    ASSERT_MESSAGE(str) \
  } \

#define ASSERT_RETURN(expr, str) \
  if (!(expr)) { \
    ASSERT_MESSAGE(str) \
    return; \
  } \

#define ASSERT_RETURN_VAL(expr, val, str) \
  if (!(expr)) { \
    ASSERT_MESSAGE(str) \
    return val; \
  } \

/* WARNING */

#define WARNING_MESSAGE(str) \
  printf ("(%s) Line %i Warning: %s\n", __FUNCTION__, __LINE__, str); \

#define WARNING_MESSAGE_FORMAT(str, ...) \
  char newstr[MAX_ERROR_LEN]; \
  sprintf (newstr, str, __VA_ARGS__); \
  WARNING_MESSAGE(newstr) \

/* ERROR */

#define ERROR_MESSAGE(str) \
  fprintf (stderr, "(%s) Line %i Error: %s\n", __FUNCTION__, __LINE__, str); \
  exit (1); \

#define ERROR_MESSAGE_FORMAT(str, ...) \
  char newstr[MAX_ERROR_LEN]; \
  sprintf (newstr, str, __VA_ARGS__); \
  ERROR_MESSAGE(newstr) \

#endif /* __MESSAGES_HPP_ */