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

#ifndef __TYPES_HPP_
#define __TYPES_HPP_

#ifdef __GNUC__
  #include <stdint.h>
#endif

#include <string>
#include <iostream>
#include <memory>

using std::cout;
using std::endl;
using std::string;
using std::shared_ptr;
using std::unique_ptr;
using std::weak_ptr;
using std::default_delete;

namespace TE {
#ifdef __GNUC__
  typedef char TEchar;
  typedef int8_t TEint8;
  typedef uint8_t TEbyte;
  typedef int16_t TEshort;
  typedef uint16_t TEushort;
  typedef int32_t TEint;
  typedef uint32_t TEuint;
  typedef int64_t TEint64;
  typedef uint64_t TEuint64;
#elif _MSC_VER
  typedef char TEchar;
  typedef signed __int8 TEint8;
  typedef unsigned __int8 TEbyte;
  typedef signed __int16 TEshort;
  typedef unsigned __int16 TEushort;
  typedef signed __int32 TEint;
  typedef unsigned __int32 TEuint;
  typedef signed __int64 TEint64;
  typedef unsigned __int64 TEuint64;
#endif

  typedef struct _TEuint128 {
    TEushort value[8];
  } TEuint128;
  
  typedef float TEfloat;
  typedef double TEdouble;
}

#endif /* __TYPES_HPP_ */
