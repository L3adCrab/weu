#ifndef WEU_ARRAY_H
#define WEU_ARRAY_H

#ifndef WEUDEF
    #ifdef WEU_STATIC
    #define WEUDEF static
    #define WEU_IMPLEMENTATION
    #else
    #define WEUDEF extern
    #endif
#endif

#include "weu_datatypes.h"

WEUDEF weu_array *weu_array_new(uint32_t length, uint32_t memberSize);
WEUDEF void weu_array_free(weu_array **h);
WEUDEF void weu_array_resize(weu_array *h, uint32_t newLength);

WEUDEF uint32_t weu_array_extend(weu_array *h, uint32_t extendBy);
WEUDEF uint32_t weu_array_reduce(weu_array *h, uint32_t reduceBy);

WEUDEF bool weu_array_isEmpty(weu_array *h);

#ifdef WEU_IMPLEMENTATION

#include <stdlib.h>
#include <memory.h>

weu_array *weu_array_new(uint32_t length, uint32_t memberSize) {
    weu_array *out = (weu_array*)malloc(sizeof(weu_array));
    out->data = calloc(length, memberSize);
    if (out->data == NULL) {
        free(out);
        return NULL;
    }
    out->length             = length;
    out->allocatedLength    = length;
    out->memberSize         = memberSize;
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
    if (newLength > h->allocatedLength) {
        h->data = realloc(h->data, h->memberSize * newLength);
        memset(h->data + (h->allocatedLength * h->memberSize), 0, h->memberSize * (newLength - h->allocatedLength));
        h->allocatedLength = newLength;
    }
    h->length = newLength;
}

uint32_t weu_array_extend(weu_array *h, uint32_t extendBy) {
    if (h == NULL) return 0;
    if (extendBy == 0) return h->length - 1;
    uint32_t posIndex = h->length;
    weu_array_resize(h, h->length + extendBy);
    return posIndex;
}
uint32_t weu_array_reduce(weu_array *h, uint32_t reduceBy) {
    if (h == NULL) return 0;
    if (reduceBy == 0) return h->length - 1;
    uint32_t posIndex = h->length - reduceBy - 1;
    if ((int32_t)posIndex < 0) posIndex = 0;
    int32_t newLen = h->length - reduceBy;
    newLen = newLen < 0 ? 0 : newLen;
    weu_array_resize(h, newLen);
    return posIndex;
}

bool weu_array_isEmpty(weu_array *h) {
    if (h == NULL) return true;
    if (h->length == 0) return true;
    return false;
}

#endif
#endif