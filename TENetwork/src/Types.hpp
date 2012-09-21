#ifndef __TYPES_HPP_
#define __TYPES_HPP_

#ifdef __GNUC__
  #include <stdint.h>
#endif

namespace TE {
#ifdef __GNUC__
  typedef char TEchar;
  typedef int8_t TEint8;
  typedef uint8_t TEbyte;
  typedef int16_t TEshort;
  typedef uint16_t TEushort;
  typedef int32_t TEint;
  typedef uint32_t TEuint;
  typedef int64_t TElong;
  typedef uint64_t TEulong;
#elif _MSC_VER
  typedef char TEchar;
  typedef signed __int8 TEint8;
  typedef unsigned __int8 TEbyte;
  typedef signed __int16 TEshort;
  typedef unsigned __int16 TEushort;
  typedef signed __int32 TEint;
  typedef unsigned __int32 TEuint;
  typedef signed __int64 TElong;
  typedef unsigned __int64 TEulong;
#endif

  typedef struct _TEuint128 {
    TEushort value[8];
  } TEuint128;
  
  typedef void* TEpointer;
  typedef float TEfloat;
  typedef double TEdouble;
}

#endif /* __TYPES_HPP_ */
