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

#ifndef weu_pair_h
#define weu_pair_h

#define WEUDEF extern

#include "weu_datatypes.h"

#include <stdlib.h>

/////////////////////////////////////////////////////////////////////////////////////////////////////
//  ALLOCATION

WEUDEF weu_pair weu_pair_new(void *v1, void *v2, datafreefun d1, datafreefun d2);
WEUDEF void weu_pair_freeData(weu_pair *h);

WEUDEF weu_pair *weu_pair_newAlloc(void *v1, void *v2, datafreefun d1, datafreefun d2);
WEUDEF void weu_pair_free(weu_pair **h);
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  DATA

WEUDEF void *weu_pair_getV1(weu_pair *h);
WEUDEF void *weu_pair_getV2(weu_pair *h);

WEUDEF void weu_pair_setV1(weu_pair *h, void* v, bool freePrevious);
WEUDEF void weu_pair_setV2(weu_pair *h, void* v, bool freePrevious);
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  DATA DESTRUCTOR

WEUDEF void weu_pair_setFreeFun(weu_pair *h, datafreefun d1, datafreefun d2);

#ifdef WEU_IMPLEMENTATION

/////////////////////////////////////////////////////////////////////////////////////////////////////
//  ALLOCATION
/////////////////////////////////////////////////////////////////////////////////////////////////////

weu_pair weu_pair_new(void *v1, void *v2, datafreefun d1, datafreefun d2) {
    weu_pair out = {v1, v2, d1, d2};
    return out;
}
void weu_pair_freeData(weu_pair *h) {
    if (h == NULL) return;
    if (h->d1 != NULL) h->d1(&h->v1);
    if (h->d2 != NULL) h->d2(&h->v2);
}

weu_pair *weu_pair_newAlloc(void *v1, void *v2, datafreefun d1, datafreefun d2) {
    weu_pair *out = (weu_pair*)malloc(sizeof(weu_pair));
    out->v1 = v1;
    out->v2 = v2;
    out->d1 = d1;
    out->d2 = d2;
    return out;
}
void weu_pair_free(weu_pair **h) {
    if (*h == NULL) return;
    weu_pair *pair = *h;
    if (pair->d1 != NULL) pair->d1(&pair->v1);
    if (pair->d2 != NULL) pair->d2(&pair->v2);
    free(*h);
    *h = NULL;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  DATA
/////////////////////////////////////////////////////////////////////////////////////////////////////

void *weu_pair_getV1(weu_pair *h) {
    return h->v1;
}
void *weu_pair_getV2(weu_pair *h) {
    return h->v2;
}

void weu_pair_setV1(weu_pair *h, void* v, bool freePrevious) {
    if (freePrevious && h->d1 != NULL) h->d1(&h->v1);
    h->v1 = v;
}
void weu_pair_setV2(weu_pair *h, void* v, bool freePrevious) {
    if (freePrevious && h->d2 != NULL) h->d2(&h->v2);
    h->v2 = v;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  DATA DESTRUCTOR
/////////////////////////////////////////////////////////////////////////////////////////////////////

void weu_pair_setFreeFun(weu_pair *h, datafreefun d1, datafreefun d2) {
    if (h == NULL) return;
    h->d1 = d1;
    h->d2 = d2;
}

#endif
#endif