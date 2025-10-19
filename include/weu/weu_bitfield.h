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

#ifndef weu_bitfield_h
#define weu_bitfield_h

#ifndef WEUDEF
    #ifdef WEU_STATIC
    #define WEUDEF static
    #define WEU_IMPLEMENTATION
    #else
    #define WEUDEF extern
    #endif
#endif

#include "weu_datatypes.h"

#include <stdlib.h>
#include <stdio.h>

#define SET_BIT8(X, N)      (X |= 0x01 << N)
#define CLEAR_BIT8(X, N)    (X &= ~0x01U << N)
#define TOGGLE_BIT8(X, N)   (X ^= 0x01 << N)
#define IS_SET_BIT8(X, N)   ((X >> N) & 0x1)

#define SET_BIT32(X, N)     (X |= 0x00000001 << N)
#define CLEAR_BIT32(X, N)   (X &= ~0x00000001U << N)
#define TOGGLE_BIT32(X, N)  (X ^= 0x00000001 << N)
#define IS_SET_BIT32(X, N)  ((X >> N) & 0x1)

#define SET_BIT64(X, N)     (X |= (weu_bitfield_64)0x0000000000000001 << N)
#define CLEAR_BIT64(X, N)   (X &= ~(weu_bitfield_64)0x0000000000000001U << N)
#define TOGGLE_BIT64(X, N)  (X ^= (weu_bitfield_64)0x0000000000000001 << N)
#define IS_SET_BIT64(X, N)  ((X >> N) & 0x1)

#define SEG8_BITC(X)        (weu_bitfield_seg8_bitCount(X))
#define SEG32_BITC(X)       (weu_bitfield_seg32_bitCount(X))
#define SEG64_BITC(X)       (weu_bitfield_seg64_bitCount(X))

/////////////////////////////////////////////////////////////////////////////////////////////////////
//  PRINT

WEUDEF void weu_bitfield_8_print(weu_bitfield_8 bf);
WEUDEF void weu_bitfield_32_print(weu_bitfield_32 bf);
WEUDEF void weu_bitfield_64_print(weu_bitfield_64 bf);

/////////////////////////////////////////////////////////////////////////////////////////////////////
//  SEGMENT BITFIELD 8 BIT
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  ALLOCATION

WEUDEF weu_bitfield_8seg *weu_bitfield_seg8_new(int segCount);
WEUDEF void weu_bitfield_seg8_resize(weu_bitfield_8seg *bf, int segCount);
WEUDEF void weu_bitfield_seg8_free(weu_bitfield_8seg **bf);
WEUDEF int weu_bitfield_seg8_bitCount(weu_bitfield_8seg *bf);
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  SET

//  example : index 10 = seccond segment, third bit 
WEUDEF void weu_bitfield_seg8_setBit(weu_bitfield_8seg *bf, int index);
WEUDEF void weu_bitfield_seg8_setBitInSeg(weu_bitfield_8seg *bf, int segIndex, int index);
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  CLEAR

//  example : index 10 = seccond segment, third bit 
WEUDEF void weu_bitfield_seg8_clearBit(weu_bitfield_8seg *bf, int index);
WEUDEF void weu_bitfield_seg8_clearBitInSeg(weu_bitfield_8seg *bf, int segIndex, int index);
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  TOGGLE

//  example : index 10 = seccond segment, third bit 
WEUDEF void weu_bitfield_seg8_toggleBit(weu_bitfield_8seg *bf, int index);
WEUDEF void weu_bitfield_seg8_toggleBitInSeg(weu_bitfield_8seg *bf, int segIndex, int index);
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  IS SET

//  example : index 10 = seccond segment, third bit 
WEUDEF int weu_bitfield_seg8_isSetBit(weu_bitfield_8seg *bf, int index);
WEUDEF int weu_bitfield_seg8_isSetBitInSeg(weu_bitfield_8seg *bf, int segIndex, int index);
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  PRINT

WEUDEF void weu_bitfield_seg8_print(weu_bitfield_8seg *bf);
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  SEGMENT BITFIELD 32 BIT
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  ALLOCATION

WEUDEF weu_bitfield_32seg *weu_bitfield_seg32_new(int segCount);
WEUDEF void weu_bitfield_seg32_resize(weu_bitfield_32seg *bf, int segCount);
WEUDEF void weu_bitfield_seg32_free(weu_bitfield_32seg **bf);
WEUDEF int weu_bitfield_seg32_bitCount(weu_bitfield_32seg *bf);
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  SET

//  example : index 34 = seccond segment, third bit 
WEUDEF void weu_bitfield_seg32_setBit(weu_bitfield_32seg *bf, int index);
WEUDEF void weu_bitfield_seg32_setBitInSeg(weu_bitfield_32seg *bf, int segIndex, int index);
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  CLEAR

//  example : index 34 = seccond segment, third bit 
WEUDEF void weu_bitfield_seg32_clearBit(weu_bitfield_32seg *bf, int index);
WEUDEF void weu_bitfield_seg32_clearBitInSeg(weu_bitfield_32seg *bf, int segIndex, int index);
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  TOGGLE

//  example : index 34 = seccond segment, third bit 
WEUDEF void weu_bitfield_seg32_toggleBit(weu_bitfield_32seg *bf, int index);
WEUDEF void weu_bitfield_seg32_toggleBitInSeg(weu_bitfield_32seg *bf, int segIndex, int index);
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  IS SET

//  example : index 34 = seccond segment, third bit 
WEUDEF int weu_bitfield_seg32_isSetBit(weu_bitfield_32seg *bf, int index);
WEUDEF int weu_bitfield_seg32_isSetBitInSeg(weu_bitfield_32seg *bf, int segIndex, int index);
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  PRINT

WEUDEF void weu_bitfield_seg32_print(weu_bitfield_32seg *bf);
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  SEGMENT BITFIELD 64 BIT
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  ALLOCATION

WEUDEF weu_bitfield_64seg *weu_bitfield_seg64_new(int segCount);
WEUDEF void weu_bitfield_seg64_resize(weu_bitfield_64seg *bf, int segCount);
WEUDEF void weu_bitfield_seg64_free(weu_bitfield_64seg **bf);
WEUDEF int weu_bitfield_seg64_bitCount(weu_bitfield_64seg *bf);
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  SET

//  example : index 66 = seccond segment, third bit 
WEUDEF void weu_bitfield_seg64_setBit(weu_bitfield_64seg *bf, int index);
WEUDEF void weu_bitfield_seg64_setBitInSeg(weu_bitfield_64seg *bf, int segIndex, int index);
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  CLEAR

//  example : index 66 = seccond segment, third bit 
WEUDEF void weu_bitfield_seg64_clearBit(weu_bitfield_64seg *bf, int index);
WEUDEF void weu_bitfield_seg64_clearBitInSeg(weu_bitfield_64seg *bf, int segIndex, int index);
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  TOGGLE

//  example : index 66 = seccond segment, third bit 
WEUDEF void weu_bitfield_seg64_toggleBit(weu_bitfield_64seg *bf, int index);
WEUDEF void weu_bitfield_seg64_toggleBitInSeg(weu_bitfield_64seg *bf, int segIndex, int index);
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  IS SET

//  example : index 66 = seccond segment, third bit 
WEUDEF int weu_bitfield_seg64_isSetBit(weu_bitfield_64seg *bf, int index);
WEUDEF int weu_bitfield_seg64_isSetBitInSeg(weu_bitfield_64seg *bf, int segIndex, int index);
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  PRINT

WEUDEF void weu_bitfield_seg64_print(weu_bitfield_64seg *bf);
WEUDEF void _print_bitfield(unsigned long long bf, unsigned long long mask, int bc);

#ifdef WEU_IMPLEMENTATION

/////////////////////////////////////////////////////////////////////////////////////////////////////
//  PRINT
/////////////////////////////////////////////////////////////////////////////////////////////////////

void weu_bitfield_8_print(weu_bitfield_8 bf) {
    _print_bitfield(bf, 0x80, 8);
    printf("\n");
}
void weu_bitfield_32_print(weu_bitfield_32 bf) {
    _print_bitfield(bf, 0x80000000, 32);
    printf("\n");
}
void weu_bitfield_64_print(weu_bitfield_64 bf) {
    _print_bitfield(bf, 0x8000000000000000, 64);
    printf("\n");
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//  SEGMENT BITFIELD 8 BIT
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  ALLOCATION
/////////////////////////////////////////////////////////////////////////////////////////////////////

weu_bitfield_8seg *weu_bitfield_seg8_new(int segCount) {
    weu_bitfield_8seg *out  = (weu_bitfield_8seg*)malloc(sizeof(weu_bitfield_8seg));
    segCount = segCount > 0 ? segCount : 0;
    out->segmentCount   = segCount;
    out->b = (weu_bitfield_8*)calloc(segCount, sizeof(weu_bitfield_8));
    return out;
}
void weu_bitfield_seg8_resize(weu_bitfield_8seg *bf, int segCount) {
    if (bf == NULL) return;
    segCount = segCount > 0 ? segCount : 0;
    int oldSize         = bf->segmentCount;
    bf->segmentCount    = segCount;
    bf->b = (weu_bitfield_8*)realloc(bf->b, segCount * sizeof(weu_bitfield_8));
    for (int i = oldSize; i < segCount; i++){
        bf->b[i] = 0;
    }
}
void weu_bitfield_seg8_free(weu_bitfield_8seg **bf) {
    if (*bf == NULL) return;
    free((*bf)->b);
    free(*bf);
    *bf = NULL;
}
int weu_bitfield_seg8_bitCount(weu_bitfield_8seg *bf) {
    return bf->segmentCount * 8;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  SET
/////////////////////////////////////////////////////////////////////////////////////////////////////

//  example : index 10 = seccond segment, third bit 
void weu_bitfield_seg8_setBit(weu_bitfield_8seg *bf, int index) {
    if (bf == NULL || index < 0 || index >= SEG8_BITC(bf)) return;
    SET_BIT8(bf->b[index / 8], index % 8);
}
void weu_bitfield_seg8_setBitInSeg(weu_bitfield_8seg *bf, int segIndex, int index) {
    if (bf == NULL || segIndex < 0 || segIndex >= bf->segmentCount || index < 0 || index > 7) return;
    SET_BIT8(bf->b[segIndex], index);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  CLEAR
/////////////////////////////////////////////////////////////////////////////////////////////////////

//  example : index 10 = seccond segment, third bit 
void weu_bitfield_seg8_clearBit(weu_bitfield_8seg *bf, int index) {
    if (bf == NULL || index < 0 || index >= SEG8_BITC(bf)) return;
    CLEAR_BIT8(bf->b[index / 8], index % 8);
}
void weu_bitfield_seg8_clearBitInSeg(weu_bitfield_8seg *bf, int segIndex, int index) {
    if (bf == NULL || segIndex < 0 || segIndex >= bf->segmentCount || index < 0 || index > 7) return;
    CLEAR_BIT8(bf->b[segIndex], index);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  TOGGLE
/////////////////////////////////////////////////////////////////////////////////////////////////////

//  example : index 10 = seccond segment, third bit 
void weu_bitfield_seg8_toggleBit(weu_bitfield_8seg *bf, int index) {
    if (bf == NULL || index < 0 || index >= SEG8_BITC(bf)) return;
    TOGGLE_BIT8(bf->b[index / 8], index % 8);
}
void weu_bitfield_seg8_toggleBitInSeg(weu_bitfield_8seg *bf, int segIndex, int index) {
    if (bf == NULL || segIndex < 0 || segIndex >= bf->segmentCount || index < 0 || index > 7) return;
    TOGGLE_BIT8(bf->b[segIndex], index);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  IS SET
/////////////////////////////////////////////////////////////////////////////////////////////////////

//  example : index 10 = seccond segment, third bit 
int weu_bitfield_seg8_isSetBit(weu_bitfield_8seg *bf, int index) {
    if (bf == NULL || index < 0 || index >= SEG8_BITC(bf)) return -1;
    return IS_SET_BIT8(bf->b[index / 8], index % 8);
}
int weu_bitfield_seg8_isSetBitInSeg(weu_bitfield_8seg *bf, int segIndex, int index) {
    if (bf == NULL || segIndex < 0 || segIndex >= bf->segmentCount || index < 0 || index > 7) return -1;
    return IS_SET_BIT8(bf->b[segIndex], index);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  PRINT
/////////////////////////////////////////////////////////////////////////////////////////////////////

void weu_bitfield_seg8_print(weu_bitfield_8seg *bf) {
    if (bf == NULL) return;
    printf("|");
    for (int i = bf->segmentCount - 1; i >= 0; i--) {
        _print_bitfield(bf->b[i], 0x80, 8);
        printf("|");
    }
    printf("\n");
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  SEGMENT BITFIELD 32 BIT
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  ALLOCATION
/////////////////////////////////////////////////////////////////////////////////////////////////////

weu_bitfield_32seg *weu_bitfield_seg32_new(int segCount) {
    weu_bitfield_32seg *out = (weu_bitfield_32seg*)malloc(sizeof(weu_bitfield_32seg));
    segCount = segCount > 0 ? segCount : 0;
    out->segmentCount   = segCount;
    out->b = (weu_bitfield_32*)calloc(segCount, sizeof(weu_bitfield_32));
    return out;
}
void weu_bitfield_seg32_resize(weu_bitfield_32seg *bf, int segCount) {
    if (bf == NULL) return;
    segCount = segCount > 0 ? segCount : 0;
    int oldSize         = bf->segmentCount;
    bf->segmentCount    = segCount;
    bf->b = (weu_bitfield_32*)realloc(bf->b, segCount * sizeof(weu_bitfield_32));
    for (int i = oldSize; i < segCount; i++){
        bf->b[i] = 0;
    }
}
void weu_bitfield_seg32_free(weu_bitfield_32seg **bf) {
    if (*bf == NULL) return;
    free((*bf)->b);
    free(*bf);
    *bf = NULL;
}
int weu_bitfield_seg32_bitCount(weu_bitfield_32seg *bf) {
    return bf->segmentCount * 32;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  SET
/////////////////////////////////////////////////////////////////////////////////////////////////////

//  example : index 34 = seccond segment, third bit 
void weu_bitfield_seg32_setBit(weu_bitfield_32seg *bf, int index) {
    if (bf == NULL || index < 0 || index >= SEG32_BITC(bf)) return;
    SET_BIT32(bf->b[index / 32], index % 32);
}
void weu_bitfield_seg32_setBitInSeg(weu_bitfield_32seg *bf, int segIndex, int index) {
    if (bf == NULL || segIndex < 0 || segIndex >= bf->segmentCount || index < 0 || index > 31) return;
    SET_BIT32(bf->b[segIndex], index);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  CLEAR
/////////////////////////////////////////////////////////////////////////////////////////////////////

//  example : index 34 = seccond segment, third bit 
void weu_bitfield_seg32_clearBit(weu_bitfield_32seg *bf, int index) {
    if (bf == NULL || index < 0 || index >= SEG32_BITC(bf)) return;
    CLEAR_BIT32(bf->b[index / 32], index % 32);
}
void weu_bitfield_seg32_clearBitInSeg(weu_bitfield_32seg *bf, int segIndex, int index) {
    if (bf == NULL || segIndex < 0 || segIndex >= bf->segmentCount || index < 0 || index > 31) return;
    CLEAR_BIT32(bf->b[segIndex], index);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  TOGGLE
/////////////////////////////////////////////////////////////////////////////////////////////////////

//  example : index 34 = seccond segment, third bit 
void weu_bitfield_seg32_toggleBit(weu_bitfield_32seg *bf, int index) {
    if (bf == NULL || index < 0 || index >= SEG32_BITC(bf)) return;
    TOGGLE_BIT32(bf->b[index / 32], index % 32);
}
void weu_bitfield_seg32_toggleBitInSeg(weu_bitfield_32seg *bf, int segIndex, int index) {
    if (bf == NULL || segIndex < 0 || segIndex >= bf->segmentCount || index < 0 || index > 31) return;
    TOGGLE_BIT32(bf->b[segIndex], index);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  IS SET
/////////////////////////////////////////////////////////////////////////////////////////////////////

//  example : index 34 = seccond segment, third bit 
int weu_bitfield_seg32_isSetBit(weu_bitfield_32seg *bf, int index) {
    if (bf == NULL || index < 0 || index >= SEG32_BITC(bf)) return -1;
    return IS_SET_BIT32(bf->b[index / 32], index % 32);
}
int weu_bitfield_seg32_isSetBitInSeg(weu_bitfield_32seg *bf, int segIndex, int index) {
    if (bf == NULL || segIndex < 0 || segIndex >= bf->segmentCount || index < 0 || index > 31) return -1;
    return IS_SET_BIT32(bf->b[segIndex], index);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  PRINT
/////////////////////////////////////////////////////////////////////////////////////////////////////

void weu_bitfield_seg32_print(weu_bitfield_32seg *bf) {
    if (bf == NULL) return;
    printf("|");
    for (int i = bf->segmentCount - 1; i >= 0; i--) {
        _print_bitfield(bf->b[i], 0x80000000, 32);
        printf("|");
    }
    printf("\n");
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  SEGMENT BITFIELD 64 BIT
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  ALLOCATION
/////////////////////////////////////////////////////////////////////////////////////////////////////

weu_bitfield_64seg *weu_bitfield_seg64_new(int segCount) {
    weu_bitfield_64seg *out = (weu_bitfield_64seg*)malloc(sizeof(weu_bitfield_64seg));
    segCount = segCount > 0 ? segCount : 0;
    out->segmentCount   = segCount;
    out->b = (weu_bitfield_64*)calloc(segCount, sizeof(weu_bitfield_64));
    return out;
}
void weu_bitfield_seg64_resize(weu_bitfield_64seg *bf, int segCount) {
    if (bf == NULL) return;
    segCount = segCount > 0 ? segCount : 0;
    int oldSize         = bf->segmentCount;
    bf->segmentCount    = segCount;
    bf->b = (weu_bitfield_64*)realloc(bf->b, segCount * sizeof(weu_bitfield_64));
    for (int i = oldSize; i < segCount; i++){
        bf->b[i] = 0;
    }
}
void weu_bitfield_seg64_free(weu_bitfield_64seg **bf) {
    if (*bf == NULL) return;
    free((*bf)->b);
    free(*bf);
    *bf = NULL;
}
int weu_bitfield_seg64_bitCount(weu_bitfield_64seg *bf) {
    return bf->segmentCount * 64;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  SET
/////////////////////////////////////////////////////////////////////////////////////////////////////

//  example : index 66 = seccond segment, third bit 
void weu_bitfield_seg64_setBit(weu_bitfield_64seg *bf, int index) {
    if (bf == NULL || index < 0 || index >= SEG64_BITC(bf)) return;
    SET_BIT64(bf->b[index / 64], index % 64);
}
void weu_bitfield_seg64_setBitInSeg(weu_bitfield_64seg *bf, int segIndex, int index) {
    if (bf == NULL || segIndex < 0 || segIndex >= bf->segmentCount || index < 0 || index > 63) return;
    SET_BIT64(bf->b[segIndex], index);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  CLEAR
/////////////////////////////////////////////////////////////////////////////////////////////////////

//  example : index 66 = seccond segment, third bit 
void weu_bitfield_seg64_clearBit(weu_bitfield_64seg *bf, int index) {
    if (bf == NULL || index < 0 || index >= SEG64_BITC(bf)) return;
    CLEAR_BIT64(bf->b[index / 64], index % 64);
}
void weu_bitfield_seg64_clearBitInSeg(weu_bitfield_64seg *bf, int segIndex, int index) {
    if (bf == NULL || segIndex < 0 || segIndex >= bf->segmentCount || index < 0 || index > 63) return;
    CLEAR_BIT64(bf->b[segIndex], index);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  TOGGLE
/////////////////////////////////////////////////////////////////////////////////////////////////////

//  example : index 66 = seccond segment, third bit 
void weu_bitfield_seg64_toggleBit(weu_bitfield_64seg *bf, int index) {
    if (bf == NULL || index < 0 || index >= SEG64_BITC(bf)) return;
    TOGGLE_BIT64(bf->b[index / 64], index % 64);
}
void weu_bitfield_seg64_toggleBitInSeg(weu_bitfield_64seg *bf, int segIndex, int index) {
    if (bf == NULL || segIndex < 0 || segIndex >= bf->segmentCount || index < 0 || index > 63) return;
    TOGGLE_BIT64(bf->b[segIndex], index);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  IS SET
/////////////////////////////////////////////////////////////////////////////////////////////////////

//  example : index 66 = seccond segment, third bit 
int weu_bitfield_seg64_isSetBit(weu_bitfield_64seg *bf, int index) {
    if (bf == NULL || index < 0 || index >= SEG64_BITC(bf)) return -1;
    return IS_SET_BIT64(bf->b[index / 64], index % 64);
}
int weu_bitfield_seg64_isSetBitInSeg(weu_bitfield_64seg *bf, int segIndex, int index) {
    if (bf == NULL || segIndex < 0 || segIndex >= bf->segmentCount || index < 0 || index > 63) return -1;
    return IS_SET_BIT64(bf->b[segIndex], index);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  PRINT
/////////////////////////////////////////////////////////////////////////////////////////////////////

void weu_bitfield_seg64_print(weu_bitfield_64seg *bf) {
    if (bf == NULL) return;
    printf("|");
    for (int i = bf->segmentCount - 1; i >= 0; i--) {
        _print_bitfield(bf->b[i], 0x8000000000000000, 64);
        printf("|");
    }
    printf("\n");
}
void _print_bitfield(unsigned long long bf, unsigned long long mask, int bc) {
    for (int i = 0; i < bc; i++) {
        printf("%i", bf & mask ? 1 : 0);
        mask >>= 1;
    }
}

#endif
#endif