#ifndef weu_bitfield_h
#define weu_bitfield_h

#ifndef WEUDEF
    #ifdef WEU_EXTERN
    #define WEUDEF extern
    #else
    #define WEUDEF static
    #define WEU_IMPLEMENTATION
    #endif
#endif

#ifdef WEU_IMPLEMENTATION

#include "weu_datatypes.h"

#include <stdlib.h>
#include <stdio.h>

#define SET_BIT8(X, N)      (X |= 0x01 << N)
#define CLEAR_BIT8(X, N)    (X &= ~0x01 << N)
#define TOGGLE_BIT8(X, N)   (X ^= 0x01 << N)
#define IS_SET_BIT8(X, N)   ((X >> N) & 0x1)

WEUDEF void weu_bitfield_8_print(bitfield_8 bf);

#define SET_BIT32(X, N)     (X |= 0x00000001 << N)
#define CLEAR_BIT32(X, N)   (X &= ~0x00000001 << N)
#define TOGGLE_BIT32(X, N)  (X ^= 0x00000001 << N)
#define IS_SET_BIT32(X, N)  ((X >> N) & 0x1)

WEUDEF void weu_bitfield_32_print(bitfield_32 bf);

#define SET_BIT64(X, N)     (X |= (bitfield_64)0x0000000000000001 << N)
#define CLEAR_BIT64(X, N)   (X &= ~(bitfield_64)0x0000000000000001 << N)
#define TOGGLE_BIT64(X, N)  (X ^= (bitfield_64)0x0000000000000001 << N)
#define IS_SET_BIT64(X, N)  ((X >> N) & 0x1)

WEUDEF void weu_bitfield_64_print(bitfield_64 bf);

#define SEG8_BITC(X)        (weu_bitfield_seg8_bitCount(X))
#define SEG32_BITC(X)       (weu_bitfield_seg32_bitCount(X))
#define SEG64_BITC(X)       (weu_bitfield_seg64_bitCount(X))

WEUDEF void qweu_bitfield_8_print(bitfield_8 bf) {
    _print_bitfield(bf, 0x80, 8);
    printf("\n");
}
WEUDEF void qweu_bitfield_32_print(bitfield_32 bf) {
    _print_bitfield(bf, 0x80000000, 32);
    printf("\n");
}
WEUDEF void qweu_bitfield_64_print(bitfield_64 bf) {
    _print_bitfield(bf, 0x8000000000000000, 64);
    printf("\n");
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//  SEGMENT BITFIELD 8 BIT
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  ALLOCATION

WEUDEF bitfield_8seg *weu_bitfield_seg8_new(int segCount) {
    bitfield_8seg *out  = (bitfield_8seg*)malloc(sizeof(bitfield_8seg));
    segCount = segCount > 0 ? segCount : 0;
    out->segmentCount   = segCount;
    out->b = (bitfield_8*)calloc(segCount, sizeof(bitfield_8));
    return out;
}
WEUDEF void weu_bitfield_seg8_resize(bitfield_8seg *bf, int segCount) {
    if (bf == NULL) return;
    segCount = segCount > 0 ? segCount : 0;
    int oldSize         = bf->segmentCount;
    bf->segmentCount    = segCount;
    bf->b = (bitfield_8*)realloc(bf->b, segCount * sizeof(bitfield_8));
    for (int i = oldSize; i < segCount; i++){
        bf->b[i] = 0;
    }
}
WEUDEF void weu_bitfield_seg8_free(bitfield_8seg **bf) {
    if (*bf == NULL) return;
    free((*bf)->b);
    free(*bf);
    *bf = NULL;
}
WEUDEF int weu_bitfield_seg8_bitCount(bitfield_8seg *bf) {
    return bf->segmentCount * 8;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  SET

//  example : index 10 = seccond segment, third bit 
WEUDEF void weu_bitfield_seg8_setBit(bitfield_8seg *bf, int index) {
    if (bf == NULL || index < 0 || index >= SEG8_BITC(bf)) return;
    SET_BIT8(bf->b[index / 8], index % 8);
}
WEUDEF void weu_bitfield_seg8_setBitInSeg(bitfield_8seg *bf, int segIndex, int index) {
    if (bf == NULL || segIndex < 0 || segIndex >= bf->segmentCount || index < 0 || index > 7) return;
    SET_BIT8(bf->b[segIndex], index);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  CLEAR

//  example : index 10 = seccond segment, third bit 
WEUDEF void weu_bitfield_seg8_clearBit(bitfield_8seg *bf, int index) {
    if (bf == NULL || index < 0 || index >= SEG8_BITC(bf)) return;
    CLEAR_BIT8(bf->b[index / 8], index % 8);
}
WEUDEF void weu_bitfield_seg8_clearBitInSeg(bitfield_8seg *bf, int segIndex, int index) {
    if (bf == NULL || segIndex < 0 || segIndex >= bf->segmentCount || index < 0 || index > 7) return;
    CLEAR_BIT8(bf->b[segIndex], index);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  TOGGLE

//  example : index 10 = seccond segment, third bit 
WEUDEF void weu_bitfield_seg8_toggleBit(bitfield_8seg *bf, int index) {
    if (bf == NULL || index < 0 || index >= SEG8_BITC(bf)) return;
    TOGGLE_BIT8(bf->b[index / 8], index % 8);
}
WEUDEF void weu_bitfield_seg8_toggleBitInSeg(bitfield_8seg *bf, int segIndex, int index) {
    if (bf == NULL || segIndex < 0 || segIndex >= bf->segmentCount || index < 0 || index > 7) return;
    TOGGLE_BIT8(bf->b[segIndex], index);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  IS SET

//  example : index 10 = seccond segment, third bit 
WEUDEF int weu_bitfield_seg8_isSetBit(bitfield_8seg *bf, int index) {
    if (bf == NULL || index < 0 || index >= SEG8_BITC(bf)) return -1;
    return IS_SET_BIT8(bf->b[index / 8], index % 8);
}
WEUDEF int weu_bitfield_seg8_isSetBitInSeg(bitfield_8seg *bf, int segIndex, int index) {
    if (bf == NULL || segIndex < 0 || segIndex >= bf->segmentCount || index < 0 || index > 7) return -1;
    return IS_SET_BIT8(bf->b[segIndex], index);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  PRINT

WEUDEF void weu_bitfield_seg8_print(bitfield_8seg *bf) {
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

WEUDEF bitfield_32seg *weu_bitfield_seg32_new(int segCount) {
    bitfield_32seg *out = (bitfield_32seg*)malloc(sizeof(bitfield_32seg));
    segCount = segCount > 0 ? segCount : 0;
    out->segmentCount   = segCount;
    out->b = (bitfield_32*)calloc(segCount, sizeof(bitfield_32));
    return out;
}
WEUDEF void weu_bitfield_seg32_resize(bitfield_32seg *bf, int segCount) {
    if (bf == NULL) return;
    segCount = segCount > 0 ? segCount : 0;
    int oldSize         = bf->segmentCount;
    bf->segmentCount    = segCount;
    bf->b = (bitfield_32*)realloc(bf->b, segCount * sizeof(bitfield_32));
    for (int i = oldSize; i < segCount; i++){
        bf->b[i] = 0;
    }
}
WEUDEF void weu_bitfield_seg32_free(bitfield_32seg **bf) {
    if (*bf == NULL) return;
    free((*bf)->b);
    free(*bf);
    *bf = NULL;
}
WEUDEF int weu_bitfield_seg32_bitCount(bitfield_32seg *bf) {
    return bf->segmentCount * 32;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  SET

//  example : index 34 = seccond segment, third bit 
WEUDEF void weu_bitfield_seg32_setBit(bitfield_32seg *bf, int index) {
    if (bf == NULL || index < 0 || index >= SEG32_BITC(bf)) return;
    SET_BIT32(bf->b[index / 32], index % 32);
}
WEUDEF void weu_bitfield_seg32_setBitInSeg(bitfield_32seg *bf, int segIndex, int index) {
    if (bf == NULL || segIndex < 0 || segIndex >= bf->segmentCount || index < 0 || index > 31) return;
    SET_BIT32(bf->b[segIndex], index);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  CLEAR

//  example : index 34 = seccond segment, third bit 
WEUDEF void weu_bitfield_seg32_clearBit(bitfield_32seg *bf, int index) {
    if (bf == NULL || index < 0 || index >= SEG32_BITC(bf)) return;
    CLEAR_BIT32(bf->b[index / 32], index % 32);
}
WEUDEF void weu_bitfield_seg32_clearBitInSeg(bitfield_32seg *bf, int segIndex, int index) {
    if (bf == NULL || segIndex < 0 || segIndex >= bf->segmentCount || index < 0 || index > 31) return;
    CLEAR_BIT32(bf->b[segIndex], index);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  TOGGLE

//  example : index 34 = seccond segment, third bit 
WEUDEF void weu_bitfield_seg32_toggleBit(bitfield_32seg *bf, int index) {
    if (bf == NULL || index < 0 || index >= SEG32_BITC(bf)) return;
    TOGGLE_BIT32(bf->b[index / 32], index % 32);
}
WEUDEF void weu_bitfield_seg32_toggleBitInSeg(bitfield_32seg *bf, int segIndex, int index) {
    if (bf == NULL || segIndex < 0 || segIndex >= bf->segmentCount || index < 0 || index > 31) return;
    TOGGLE_BIT32(bf->b[segIndex], index);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  IS SET

//  example : index 34 = seccond segment, third bit 
WEUDEF int weu_bitfield_seg32_isSetBit(bitfield_32seg *bf, int index) {
    if (bf == NULL || index < 0 || index >= SEG32_BITC(bf)) return -1;
    return IS_SET_BIT32(bf->b[index / 32], index % 32);
}
WEUDEF int weu_bitfield_seg32_isSetBitInSeg(bitfield_32seg *bf, int segIndex, int index) {
    if (bf == NULL || segIndex < 0 || segIndex >= bf->segmentCount || index < 0 || index > 31) return -1;
    return IS_SET_BIT32(bf->b[segIndex], index);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  PRINT

WEUDEF void weu_bitfield_seg32_print(bitfield_32seg *bf) {
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

WEUDEF bitfield_64seg *weu_bitfield_seg64_new(int segCount) {
    bitfield_64seg *out = (bitfield_64seg*)malloc(sizeof(bitfield_64seg));
    segCount = segCount > 0 ? segCount : 0;
    out->segmentCount   = segCount;
    out->b = (bitfield_64*)calloc(segCount, sizeof(bitfield_64));
    return out;
}
WEUDEF void weu_bitfield_seg64_resize(bitfield_64seg *bf, int segCount) {
    if (bf == NULL) return;
    segCount = segCount > 0 ? segCount : 0;
    int oldSize         = bf->segmentCount;
    bf->segmentCount    = segCount;
    bf->b = (bitfield_64*)realloc(bf->b, segCount * sizeof(bitfield_64));
    for (int i = oldSize; i < segCount; i++){
        bf->b[i] = 0;
    }
}
WEUDEF void weu_bitfield_seg64_free(bitfield_64seg **bf) {
    if (*bf == NULL) return;
    free((*bf)->b);
    free(*bf);
    *bf = NULL;
}
WEUDEF int weu_bitfield_seg64_bitCount(bitfield_64seg *bf) {
    return bf->segmentCount * 64;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  SET

//  example : index 66 = seccond segment, third bit 
WEUDEF void weu_bitfield_seg64_setBit(bitfield_64seg *bf, int index) {
    if (bf == NULL || index < 0 || index >= SEG64_BITC(bf)) return;
    SET_BIT64(bf->b[index / 64], index % 64);
}
WEUDEF void weu_bitfield_seg64_setBitInSeg(bitfield_64seg *bf, int segIndex, int index) {
    if (bf == NULL || segIndex < 0 || segIndex >= bf->segmentCount || index < 0 || index > 63) return;
    SET_BIT64(bf->b[segIndex], index);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  CLEAR

//  example : index 66 = seccond segment, third bit 
WEUDEF void weu_bitfield_seg64_clearBit(bitfield_64seg *bf, int index) {
    if (bf == NULL || index < 0 || index >= SEG64_BITC(bf)) return;
    CLEAR_BIT64(bf->b[index / 64], index % 64);
}
WEUDEF void weu_bitfield_seg64_clearBitInSeg(bitfield_64seg *bf, int segIndex, int index) {
    if (bf == NULL || segIndex < 0 || segIndex >= bf->segmentCount || index < 0 || index > 63) return;
    CLEAR_BIT64(bf->b[segIndex], index);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  TOGGLE

//  example : index 66 = seccond segment, third bit 
WEUDEF void weu_bitfield_seg64_toggleBit(bitfield_64seg *bf, int index) {
    if (bf == NULL || index < 0 || index >= SEG64_BITC(bf)) return;
    TOGGLE_BIT64(bf->b[index / 64], index % 64);
}
WEUDEF void weu_bitfield_seg64_toggleBitInSeg(bitfield_64seg *bf, int segIndex, int index) {
    if (bf == NULL || segIndex < 0 || segIndex >= bf->segmentCount || index < 0 || index > 63) return;
    TOGGLE_BIT64(bf->b[segIndex], index);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  IS SET

//  example : index 66 = seccond segment, third bit 
WEUDEF int weu_bitfield_seg64_isSetBit(bitfield_64seg *bf, int index) {
    if (bf == NULL || index < 0 || index >= SEG64_BITC(bf)) return -1;
    return IS_SET_BIT64(bf->b[index / 64], index % 64);
}
WEUDEF int weu_bitfield_seg64_isSetBitInSeg(bitfield_64seg *bf, int segIndex, int index) {
    if (bf == NULL || segIndex < 0 || segIndex >= bf->segmentCount || index < 0 || index > 63) return -1;
    return IS_SET_BIT64(bf->b[segIndex], index);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  PRINT

WEUDEF void weu_bitfield_seg64_print(bitfield_64seg *bf) {
    if (bf == NULL) return;
    printf("|");
    for (int i = bf->segmentCount - 1; i >= 0; i--) {
        _print_bitfield(bf->b[i], 0x8000000000000000, 64);
        printf("|");
    }
    printf("\n");
}

WEUDEF void _print_bitfield(unsigned long long bf, unsigned long long mask, int bc) {
    for (int i = 0; i < bc; i++) {
        printf("%i", bf & mask ? 1 : 0);
        mask >>= 1;
    }
}

#endif
#endif