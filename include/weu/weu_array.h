/*///////////////////////////////////////////////////////////////////////////////////
//  SPDX-License-Identifier: Unlicense
/////////////////////////////////////////////////////////////////////////////////////
//  USAGE
//  Functions are defined as extern.
//  To implement somewhere in source file before including header file
//  #define WEU_IMPLEMENTATION
//  Implementation should be defined once.
//
//  #define WEU_IMPLEMENTATION
//  #include <path_to_lib/weu_master.h>
//
//  To include all weu library in souce file at once, include weu_master.h
*////////////////////////////////////////////////////////////////////////////////////

#ifndef WEU_ARRAY_H
#define WEU_ARRAY_H

#define WEUDEF extern

#include "weu_datatypes.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////
//  ALLOCATION

WEUDEF weu_array *weu_array_new(uint32_t capacity, uint32_t sizeOfData);
WEUDEF void weu_array_free(weu_array **h);
WEUDEF void weu_array_resize(weu_array *h, uint32_t newLength);
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  DATA

WEUDEF void weu_array_getAt(weu_array *h, uint32_t index, void *out);
WEUDEF void weu_array_setAt(weu_array *h, uint32_t index, void *data);
WEUDEF void weu_array_clearData(weu_array *h, uint32_t index);

WEUDEF bool weu_array_isEmpty(weu_array *h);
WEUDEF void weu_array_clear(weu_array *h);
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  END

WEUDEF void weu_array_push(weu_array *h, void *data);
WEUDEF void weu_array_pop(weu_array *h, void *out);
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  BEG

WEUDEF void weu_array_unshift(weu_array *h, void *data);
WEUDEF void weu_array_shift(weu_array *h, void *out);

#ifdef WEU_IMPLEMENTATION

#include <stdlib.h>
#include <memory.h>

/////////////////////////////////////////////////////////////////////////////////////////////////////
//  ALLOCATION
/////////////////////////////////////////////////////////////////////////////////////////////////////

weu_array *weu_array_new(uint32_t capacity, uint32_t sizeOfData) {
    weu_array *out = (weu_array*)malloc(sizeof(weu_array));
    out->data = calloc(capacity, sizeOfData);
    if (out->data == NULL) {
        free(out);
        return NULL;
    }
    out->count      = 0;
    out->capacity   = capacity;
    out->dataSize   = sizeOfData;
    return out;
}
void weu_array_free(weu_array **h) {
    if (*h == NULL) return;
    free((*h)->data);
    free(*h);
    *h = NULL;
}
void weu_array_resize(weu_array *h, uint32_t newLength) {
    if (h == NULL) return;
    if (newLength > h->capacity) {
        h->data = realloc(h->data, h->dataSize * newLength);
        memset(h->data + (h->capacity * h->dataSize), 0, h->dataSize * (newLength - h->capacity));
        h->capacity = newLength;
    }
    h->count = newLength;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  DATA
/////////////////////////////////////////////////////////////////////////////////////////////////////

void weu_array_getAt(weu_array *h, uint32_t index, void *out) {
    if (h == NULL || out == NULL) { out = NULL; return; }
    if (h->count == 0 || index > h->count - 1) { out = NULL; return; }
}
void weu_array_setAt(weu_array *h, uint32_t index, void *data) {
    if (h == NULL) return;
    if (index >= h->count) return;
    memmove(h->data + (h->dataSize * index), data, h->dataSize);
}
void weu_array_clearData(weu_array *h, uint32_t index) {
    if (h == NULL) return;
    if (index >= h->count) return;
    memset(h->data + (h->dataSize * index), 0, h->dataSize);
}

bool weu_array_isEmpty(weu_array *h) {
    if (h == NULL) return true;
    if (h->count == 0) return true;
    return false;
}
void weu_array_clear(weu_array *h) {
    if (h == NULL) return;
    memset(h->data, 0, h->dataSize * h->capacity);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  BEG
/////////////////////////////////////////////////////////////////////////////////////////////////////

void weu_array_unshift(weu_array *h, void *data) {
    if (h == NULL) return;
    uint32_t oldLen = h->count;
    weu_array_resize(h, oldLen + 1);
    memmove(h->data + h->dataSize, h->data, h->dataSize * oldLen);
    memcpy(h->data, data, h->dataSize);
}
void weu_array_shift(weu_array *h, void *out) {
    if (h == NULL) return;
    if (h->count == 0) { out = NULL; return; }
    if (out != NULL) memcpy(out, h->data, h->dataSize);
    memmove(h->data, h->data + h->dataSize, (h->count - 1) * h->dataSize);
    weu_array_resize(h, h->count - 1);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  END
/////////////////////////////////////////////////////////////////////////////////////////////////////

void weu_array_push(weu_array *h, void *data) {
    if (h == NULL) return;
    weu_array_resize(h, h->count + 1);
    weu_array_setAt(h, h->count - 1, data);
}
void weu_array_pop(weu_array *h, void *out) {
    if (h == NULL) return;
    if (h->count == 0) { out = NULL; return; }
    if (out != NULL) memcpy(out, h->data + ((h->count - 1) * h->dataSize), h->dataSize);
    weu_array_resize(h, h->count - 1);
}

#endif
#endif