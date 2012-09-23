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
  char newstr[MAX_LEN]; \
  sprintf (newstr, str, __VA_ARGS__); \
  WARNING_MESSAGE(newstr) \

/* ERROR */

#define ERROR_MESSAGE(str) \
  fprintf (stderr, "(%s) Line %i Error: %s\n", __FUNCTION__, __LINE__, str); \
  exit (1); \

#define ERROR_MESSAGE_FORMAT(str, ...) \
  char newstr[MAX_LEN]; \
  sprintf (newstr, str, __VA_ARGS__); \
  ERROR_MESSAGE(newstr) \

#endif /* __MESSAGES_HPP_ */