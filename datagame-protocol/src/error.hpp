#ifndef ERROR_HPP
#define ERROR_HPP

#include <stdio.h>

#define MAX_ERROR_LEN 256

#define ERROR_MESSAGE(str) \
  fprintf (stderr, "(%s) Line %i Error: %s\n", __FUNCTION__, __LINE__, str); \

#define ERROR_MESSAGE_FORMAT(str, ...) \
  char newstr[MAX_ERROR_LEN]; \
  sprintf (newstr, str, __VA_ARGS__); \
  ERROR_MESSAGE(newstr) \
  exit (1); \

#endif // ERROR_HPP