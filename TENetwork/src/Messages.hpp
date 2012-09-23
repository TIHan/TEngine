/*
  Copyright (c) 2012, William F. Smith
  All rights reserved.

  Redistribution and use in source and binary forms, with or without modification,
  are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice,
  this list of conditions and the following disclaimer.

  * Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation 
  and/or other materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE 
  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
  THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef __MESSAGES_HPP_
#define __MESSAGES_HPP_

#include <stdio.h>
#include <cstdlib>

#define MAX_LEN 256

/* ASSERT */

#define ASSERT_MESSAGE(str) \
  printf ("(%s) Line %i Assertion: %s\n", __FUNCTION__, __LINE__, str);

#define ASSERT_MESSAGE_FORMAT(str, ...) \
  char newstr[MAX_LEN]; \
  sprintf (newstr, str, __VA_ARGS__); \
  ASSERT_MESSAGE(newstr)

#define ASSERT(expr, str) \
  if (!(expr)) { \
    ASSERT_MESSAGE(str) \
  }

#define ASSERT_FORMAT(expr, str, ...) \
  if (!(expr)) { \
    ASSERT_MESSAGE_FORMAT(str, __VA_ARGS__) \
  }

#define ASSERT_RETURN(expr, str) \
  if (!(expr)) { \
    ASSERT_MESSAGE(str) \
    return; \
  }

#define ASSERT_RETURN_FORMAT(expr, str, ...) \
  if (!(expr)) { \
    ASSERT_MESSAGE_FORMAT(str, __VA_ARGS__) \
    return; \
  }

#define ASSERT_RETURN_VAL(expr, val, str) \
  if (!(expr)) { \
    ASSERT_MESSAGE(str) \
    return val; \
  }

#define ASSERT_RETURN_VAL_FORMAT(expr, val, str, ...) \
  if (!(expr)) { \
    ASSERT_MESSAGE_FORMAT(str, __VA_ARGS__) \
    return val; \
  }

/* WARNING */

#define WARNING_MESSAGE(str) \
  printf ("(%s) Line %i Warning: %s\n", __FUNCTION__, __LINE__, str); \

#define WARNING_MESSAGE_FORMAT(str, ...) \
  char newstr[MAX_LEN]; \
  sprintf (newstr, str, __VA_ARGS__); \
  WARNING_MESSAGE(newstr)

#define WARNING_IF(expr, str) \
  if ((expr)) { \
    WARNING_MESSAGE(str) \
  }

#define WARNING_IF_FORMAT(expr, str, ...) \
  if ((expr)) { \
    WARNING_MESSAGE_FORMAT(str, __VA_ARGS__) \
  }

#define WARNING_IF_RETURN(expr, str) \
  if ((expr)) { \
    WARNING_MESSAGE(str) \
    return; \
  }

#define WARNING_IF_RETURN_FORMAT(expr, str, ...) \
  if ((expr)) { \
    WARNING_MESSAGE_FORMAT(str, __VA_ARGS__) \
    return; \
  }

#define WARNING_IF_RETURN_VAL(expr, val, str) \
  if ((expr)) { \
    WARNING_MESSAGE(str) \
    return val; \
  }

#define WARNING_IF_RETURN_VAL_FORMAT(expr, val, str, ...) \
  if ((expr)) { \
    WARNING_MESSAGE_FORMAT(str, __VA_ARGS__) \
    return val; \
  }

/* ERROR */

#ifdef __GNUC__
  #define ERROR_MESSAGE(str) \
    fprintf (stderr, "(%s) Line %i Error: %s\n", __FUNCTION__, __LINE__, str); \
    exit (1);
#elif _MSC_VER
  #define ERROR_MESSAGE(str) \
    fprintf (stderr, "(%s) Line %i Error: %s\n", __FUNCTION__, __LINE__, str); \
    system ("pause"); \
    exit (1);
#endif

#define ERROR_MESSAGE_FORMAT(str, ...) \
  char newstr[MAX_LEN]; \
  sprintf (newstr, str, __VA_ARGS__); \
  ERROR_MESSAGE(newstr)

#define ERROR_IF(expr, str) \
  if ((expr)) { \
    ERROR_MESSAGE(str) \
  }

#define ERROR_IF_FORMAT(expr, str, ...) \
  if ((expr)) { \
    ERROR_MESSAGE_FORMAT(str, __VA_ARGS__) \
  }

#endif /* __MESSAGES_HPP_ */