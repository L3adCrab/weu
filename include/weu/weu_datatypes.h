/*///////////////////////////////////////////////////////////////////////////////////
//  SPDX-License-Identifier: Unlicense
/////////////////////////////////////////////////////////////////////////////////////
//  USAGE
//  By default functions are defined as extern.
//  To implement somewhere in source file before including header file
//  #define WEU_IMPLEMENTATION
//  Implementation should be defined only once.
//  
//  For use as static functions before including header file
//  #define WEU_STATIC
//  There if no need to define WEU_IMPLEMENTATION when using as static,
//  although WEU_STATIC will need to be somewhere defined in every source file where
//  library will be used. To circumvent this and whole library will be used as static
//  add the WEU_STATIC define to compiler (gcc/clang - -DWEU_STATIC)
//
//  To include all weu library in souce file at once, include weu_master.h  
*////////////////////////////////////////////////////////////////////////////////////

#ifndef weu_datatypes_h
#define weu_datatypes_h

#ifndef WEUDEF
    #ifdef WEU_STATIC
        #define WEUDEF static
        #define WEU_IMPLEMENTATION
    #else
        #define WEUDEF extern
    #endif
#endif

#include <stdint.h>
#include <stdbool.h>

#ifndef SWAPVAR
#define SWAPVAR(A, B, TMP) { TMP = B; B = A; A = TMP; }
#endif

// Function pointer used for freeing memory of data contained in list, pair and hashtable.
// If data is construced in a way free function from stdlib is enough pass - weu_stdFree
// 
// For data where stdFree is not viable example :
//
// void example_fun(void **handle) {
//   free(((datatype*)*handle)->data);
//   free(*handle);
//   **handle = NULL;
// }

/////////////////////////////////////////////////////////////////////////////////////////////////////
//  FUNCTIONS POINTERS

typedef void (*datafreefun) ( void**);
typedef bool (*datacompfun) ( void*, void* );
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  BITFIELD

typedef uint8_t                     weu_bitfield_8;
typedef uint32_t                    weu_bitfield_32;
typedef uint64_t                    weu_bitfield_64;

typedef struct weu_bitfield_8seg    { char segmentCount; weu_bitfield_8  *b; }  weu_bitfield_8seg;
typedef struct weu_bitfield_32seg   { char segmentCount; weu_bitfield_32 *b; }  weu_bitfield_32seg;
typedef struct weu_bitfield_64seg   { char segmentCount; weu_bitfield_64 *b; }  weu_bitfield_64seg;
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  STRING

typedef struct weu_string           { uint32_t length; char *text; uint32_t charPtrPos; uint32_t allocLength; }    weu_string;
// string no allocation
// Stores up to 511 characters,
// 512 including null terminator.
typedef struct weu_stringNA         { uint32_t length; char text[512]; }                     weu_stringNA;
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  LIST

typedef struct weu_list             { uint32_t length; uint32_t allocatedLength; void **data; datafreefun d; } weu_list;
typedef struct weu_array            { uint32_t length; uint32_t allocatedLength; uint32_t memberSize; void *data; } weu_array;
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  PAIR

typedef struct weu_pair             { void *v1, *v2; datafreefun d1, d2; }      weu_pair;
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  HASHTABLE

typedef struct weu_hashItem         { weu_string *key; void *value; bool inUse; }           weu_hashItem;
typedef struct weu_hashTable        { uint32_t length; weu_hashItem *data; datafreefun d; } weu_hashTable;
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  ALLOCATE PRIMARY TYPES

WEUDEF int8_t   *weu_allocChar(int8_t value);
WEUDEF uint8_t  *weu_allocUChar(uint8_t value);
WEUDEF int32_t  *weu_allocInt(int32_t value);
WEUDEF uint32_t *weu_allocUInt(uint32_t value);
WEUDEF int64_t  *weu_allocLong(int64_t value);
WEUDEF uint64_t *weu_allocULong(uint64_t value);
WEUDEF float    *weu_allocFloat(float value);

#ifdef WEU_IMPLEMENTATION

#include <stdlib.h>

int8_t   *weu_allocChar(int8_t value) {
    int8_t *out = (int8_t*)malloc(sizeof(int8_t));
    *out = value;
    return out;
}
uint8_t  *weu_allocUChar(uint8_t value) {
    uint8_t *out = (uint8_t*)malloc(sizeof(uint8_t));
    *out = value;
    return out;
}
int32_t *weu_allocInt(int32_t value) {
    int32_t *out = (int32_t*)malloc(sizeof(int32_t));
    *out = value;
    return out;
}
uint32_t *weu_allocUInt(uint32_t value) {
    uint32_t *out = (uint32_t*)malloc(sizeof(uint32_t));
    *out = value;
    return out;
}
int64_t *weu_allocLong(int64_t value) {
    int64_t *out = (int64_t*)malloc(sizeof(int64_t));
    *out = value;
    return out;
}
uint64_t *weu_allocULong(uint64_t value) {
    uint64_t *out = (uint64_t*)malloc(sizeof(uint64_t));
    *out = value;
    return out;
}
float *weu_allocFloat(float value) {
    float *out = (float*)malloc(sizeof(float));
    *out = value;
    return out;
}

#endif
#endif