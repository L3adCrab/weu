#ifndef weu_datatypes_h
#define weu_datatypes_h

#include <stdint.h>
#include <stdbool.h>

#ifndef ALLOC_TYPES
#define ALLOC_TYPES
#define ALLOC_UNDEF         0
#define ALLOC_STACK         1
#define ALLOC_MALLOC        2
// Reference pointer - &data_t*
#define ALLOC_REF2PT        3
#endif

typedef uint8_t                     weu_bitfield_8;
typedef uint32_t                    weu_bitfield_32;
typedef uint64_t                    weu_bitfield_64;

typedef struct weu_bitfield_8seg    { char segmentCount; weu_bitfield_8  *b; }  weu_bitfield_8seg;
typedef struct weu_bitfield_32seg   { char segmentCount; weu_bitfield_32 *b; }  weu_bitfield_32seg;
typedef struct weu_bitfield_64seg   { char segmentCount; weu_bitfield_64 *b; }  weu_bitfield_64seg;

typedef void   (*datafreefun)       ( void**);
typedef struct weu_list             { int length; void **data; char allocType; datafreefun d; } weu_list;

typedef struct weu_string           { int length; char *text; }                                 weu_string;

typedef struct weu_hashItem         { weu_string *key; void *value; bool inUse; }                weu_hashItem;
typedef struct weu_hashTable        { int length; weu_hashItem *data; datafreefun d; }          weu_hashTable;

#endif