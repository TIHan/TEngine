#ifndef ERROR_HPP
#define ERROR_HPP

#include <stdio.h>

#define MAX_ERROR_LEN 256

#if _MSC_VER
  #define snprintf _snprintf_s
#endif

#define ERROR_MESSAGE(str) \
  fprintf (stderr, "(%s) Error: %s at line %i.\n", __FUNCTION__, str, __LINE__); \

#define ERROR_MESSAGE_FORMAT(str, ...) \
  char newstr[MAX_ERROR_LEN]; \
  snprintf (newstr, MAX_ERROR_LEN, str, __VA_ARGS__); \
  ERROR_MESSAGE(newstr) \

#endif // ERROR_HPP