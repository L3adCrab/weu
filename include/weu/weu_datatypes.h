#ifndef weu_datatypes_h
#define weu_datatypes_h

#ifndef FALSE
#define FALSE   0
#endif
#ifndef TRUE
#define TRUE    1
#endif

#ifndef ALLOC_TYPES
#define ALLOC_TYPES
#define ALLOC_UNDEF         0
#define ALLOC_STACK         1
#define ALLOC_MALLOC        2
// Reference pointer - &data_t*
#define ALLOC_REF2PT        3
#endif

typedef unsigned char           bitfield_8;
typedef unsigned int            bitfield_32;
typedef unsigned long long      bitfield_64;

typedef struct bitfield_8seg    { char segmentCount; bitfield_8  *b; }                          bitfield_8seg;
typedef struct bitfield_32seg   { char segmentCount; bitfield_32 *b; }                          bitfield_32seg;
typedef struct bitfield_64seg   { char segmentCount; bitfield_64 *b; }                          bitfield_64seg;

typedef void   (*datafreefun)   ( void**);
typedef struct list             { int length; void **data; char allocType; datafreefun d; }     list;

typedef struct string           { int length; char *text; }                                     string;

typedef struct hashItem         { string *key; void *value; int inUse; }                        hashItem;
typedef struct hashTable        { int length; hashItem *data; datafreefun d; }                  hashTable;

#endif