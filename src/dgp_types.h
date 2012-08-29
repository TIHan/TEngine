#ifdef __GNUC__
  typedef int8_t dgp_int8;
  typedef uint8_t dgp_uint8;
  typedef int16_t dgp_int16;
  typedef uint16_t dgp_uint16;
  typedef int32_t dgp_int32;
  typedef uint32_t dgp_uint32;
  typedef int64_t dgp_int64;
  typedef uint64_t dgp_uint64;
  
  typedef struct _dgp_uint128
  {
    dgp_uint16 value[8];
  } dgp_uint128;
  
  typedef void* dgp_pointer;
  typedef float dgp_float;
  typedef double dgp_double;
#endif
  
typedef enum { FALSE, TRUE } dgp_boolean;
