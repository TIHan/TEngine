#ifndef TYPES_HPP
#define TYPES_HPP

#ifdef __GNUC__
  #include <stdint.h>
#endif

namespace dgp {
#ifdef __GNUC__
  typedef char dgpChar;
  typedef int8_t dgpInt8;
  typedef uint8_t dgpByte;
  typedef int16_t dgpShort;
  typedef uint16_t dgpUshort;
  typedef int32_t dgpInt;
  typedef uint32_t dgpUint;
  typedef int64_t dgpLong;
  typedef uint64_t dgpUlong;
#elif _MSC_VER
  typedef char dgpChar;
  typedef signed __int8 dgpInt8;
  typedef unsigned __int8 dgpByte;
  typedef signed __int16 dgpShort;
  typedef unsigned __int16 dgpUshort;
  typedef signed __int32 dgpInt;
  typedef unsigned __int32 dgpUint;
  typedef signed __int64 dgpLong;
  typedef unsigned __int64 dgpUlong;
#endif

  typedef struct _dgpUint128 {
    dgpUshort value[8];
  } dgpUint128;
  
  typedef void* dgpPointer;
  typedef float dgpFloat;
  typedef double dgpDouble;
}

#endif // TYPES_HPP
