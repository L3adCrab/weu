#ifndef weu_datatypes_h
#define weu_datatypes_h

/*///////////////////////////////////////////////////////////////////////////////////
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

#include <stdint.h>
#include <stdbool.h>

typedef void   (*datafreefun)       ( void**);
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  BITFIELD

typedef uint8_t                     weu_bitfield_8;
typedef uint32_t                    weu_bitfield_32;
typedef uint64_t                    weu_bitfield_64;

typedef struct weu_bitfield_8seg    { char segmentCount; weu_bitfield_8  *b; }  weu_bitfield_8seg;
typedef struct weu_bitfield_32seg   { char segmentCount; weu_bitfield_32 *b; }  weu_bitfield_32seg;
typedef struct weu_bitfield_64seg   { char segmentCount; weu_bitfield_64 *b; }  weu_bitfield_64seg;
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  LIST

typedef struct weu_list             { int length; void **data; datafreefun d; } weu_list;
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  STRING

typedef struct weu_string           { int length; char *text; uint32_t charPtrPos; }    weu_string;
// string no allocation
// Stores up to 511 characters,
// 512 including null terminator.
typedef struct weu_stringNA         { int length; char text[512];} weu_stringNA;
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  HASHTABLE

typedef struct weu_hashItem         { weu_string *key; void *value; bool inUse; }       weu_hashItem;
typedef struct weu_hashTable        { int length; weu_hashItem *data; datafreefun d; }  weu_hashTable;
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  PAIR

typedef struct weu_pair             { void *v1, *v2; datafreefun d1, d2; }  weu_pair;

#endif