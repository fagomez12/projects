/**
 * @file tyepedef_header typedef_lib.h
 * @brief This file contains all the typedef used in the libreries
 * @date July 2025
 * @author Felipe A. Gómez Olaya
 */

#ifndef LIB_TYPEDEF
#define LIB_TYPEDEF

#ifdef __cplusplus
extern "C"
{
#endif

    typedef short int16;
    typedef long int32;
    typedef unsigned char Uint8;
    typedef char int8;
    typedef unsigned short Uint16;
    typedef unsigned long Uint32;
    typedef float float32;
    typedef long double float64;
    //
    // C99 defines boolean type to be _Bool, but this doesn't match the format of
    // the other standard integer types.  bool_t has been defined to fill this gap.
    //
    typedef _Bool bool_t;

    //
    // used for a bool function return status
    //
    typedef _Bool status_t;

#ifndef SUCCESS
#define SUCCESS true
#endif

#ifndef FAIL
#define FAIL false
#endif
//
// C++ Bool Compatibility
//
#if defined(__cplusplus)
    typedef bool _Bool;
#endif
#ifdef __cplusplus
}
#endif // extern "C"
#endif
