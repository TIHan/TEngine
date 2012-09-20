#ifndef WARNING_HPP
#define WARNING_HPP

#include <stdio.h>

#define MAX_ERROR_LEN 256

#define WARNING_MESSAGE(str) \
  printf ("(%s) Line %i Warning: %s\n", __FUNCTION__, __LINE__, str); \

#define WARNING_MESSAGE_FORMAT(str, ...) \
  char newstr[MAX_ERROR_LEN]; \
  sprintf (newstr, str, __VA_ARGS__); \
  WARNING_MESSAGE(newstr) \

#endif // WARNING