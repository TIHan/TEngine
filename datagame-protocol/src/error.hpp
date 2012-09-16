#ifndef ERROR_HPP
#define ERROR_HPP

#include <stdio.h>

#define ERROR_MESSAGE(str) \
  fprintf (stderr, "(%s) Error: %s at line %i.\n", __FUNCTION__, str, __LINE__); \

#define ERROR_MESSAGE_FORMAT(str, ...) \
  char newstr[255]; \
  sprintf_s (newstr, str, __VA_ARGS__); \
  ERROR_MESSAGE(newstr) \

#endif // ERROR_HPP