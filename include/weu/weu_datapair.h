/*///////////////////////////////////////////////////////////////////////////////////
//  SPDX-License-Identifier: Unlicense
/////////////////////////////////////////////////////////////////////////////////////
//  IMPLEMENTATION
//
//  Functions are defined as extern.
//  To implement somewhere in source file before including header file
//  #define WEU_IMPLEMENTATION
//  Implementation should be defined once.
//
//  #define WEU_IMPLEMENTATION
//  #include <path_to_lib/weu_master.h>
//
//  To include all weu library in souce file at once, include weu_master.h
/////////////////////////////////////////////////////////////////////////////////////
//  USAGE
//
//  Save memory to pair data.
//  Not to be used to save pointers, for pointers use weu_pair
*////////////////////////////////////////////////////////////////////////////////////

#ifndef WEU_DATA_PAIR_H
#define WEU_DATA_PAIR_H

#define WEUDEF extern

#include "weu_datatypes.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////
//  ALLOCATION

WEUDEF weu_dataPair *weu_dataPair_new(uint32_t dataSize_1, uint32_t dataSize_2);
WEUDEF void weu_dataPair_free(weu_dataPair **h);
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  GET DATA

WEUDEF void weu_dataPair_getData(weu_dataPair *h, void *out1, void *out2);
WEUDEF void weu_dataPair_getData1(weu_dataPair *h, void *out);
WEUDEF void weu_dataPair_getData2(weu_dataPair *h, void *out);
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  SET DATA

WEUDEF void weu_dataPair_setData(weu_dataPair *h, void *data1, void *data2);
WEUDEF void weu_dataPair_setData1(weu_dataPair *h, void *data);
WEUDEF void weu_dataPair_setData2(weu_dataPair *h, void *data);
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  RESIZE DATA

WEUDEF void weu_dataPair_resizeData(weu_dataPair *h, uint32_t dataSize1, uint32_t dataSize2);
WEUDEF void weu_dataPair_resizeData1(weu_dataPair *h, uint32_t dataSize);
WEUDEF void weu_dataPair_resizeData2(weu_dataPair *h, uint32_t dataSize);

#ifdef WEU_IMPLEMENTATION

#include <stdlib.h>
#include <memory.h>

/////////////////////////////////////////////////////////////////////////////////////////////////////
//  ALLOCATION
/////////////////////////////////////////////////////////////////////////////////////////////////////
weu_dataPair *weu_dataPair_new(uint32_t dataSize_1, uint32_t dataSize_2) {
    weu_dataPair *out = (weu_dataPair*)malloc(sizeof(weu_dataPair));
    out->dataSize1 = dataSize_1;
    out->dataSize2 = dataSize_2;
    out->data = calloc(1, dataSize_1 + dataSize_2);
    return out;
}
void weu_dataPair_free(weu_dataPair **h) {
    if (*h == NULL) return;
    free((*h)->data);
    free(*h);
    *h = NULL;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  GET DATA

void weu_dataPair_getData(weu_dataPair *h, void *out1, void *out2) {
    if (h == NULL) { out1 = NULL; out2 = NULL; return; }
    memcpy(out1, h->data, h->dataSize1);
    memcpy(out2, h->data + h->dataSize1, h->dataSize2);
}
void weu_dataPair_getData1(weu_dataPair *h, void *out) {
    if (h == NULL) { out = NULL; return; }
    memcpy(out, h->data, h->dataSize1);
}
void weu_dataPair_getData2(weu_dataPair *h, void *out) {
    if (h == NULL) { out = NULL; return; }
    memcpy(out, h->data + h->dataSize1, h->dataSize2);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  SET DATA

void weu_dataPair_setData(weu_dataPair *h, void *data1, void *data2) {
    if (h != NULL) return;
    memcpy(h->data, data1, h->dataSize1);
    memcpy(h->data + h->dataSize1, data2, h->dataSize2);
}
void weu_dataPair_setData1(weu_dataPair *h, void *data) {
    if (h == NULL) return;
    memcpy(h->data, data, h->dataSize1);
    // h->data = data;
}
void weu_dataPair_setData2(weu_dataPair *h, void *data) {
    if (h == NULL) return;
    memcpy(h->data + h->dataSize1, data, h->dataSize2);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  RESIZE DATA

void weu_dataPair_resizeData(weu_dataPair *h, uint32_t dataSize1, uint32_t dataSize2) {
    if (h == NULL) return;
    h->data = realloc(h->data, dataSize1 + dataSize2);
    memset(h->data, 0, h->dataSize1 + h->dataSize2);
}
void weu_dataPair_resizeData1(weu_dataPair *h, uint32_t dataSize) {
    if (h == NULL) return;
    if (dataSize < h->dataSize1) {
        memmove(h->data + dataSize, h->data + h->dataSize1, h->dataSize2);
        h->data = realloc(h->data, dataSize + h->dataSize2);
    }
    else {
        h->data = realloc(h->data, dataSize + h->dataSize2);
        memmove(h->data + dataSize, h->data + h->dataSize1, h->dataSize2);
    }
    memset(h->data, 0, dataSize);
    h->dataSize1 = dataSize;
}
void weu_dataPair_resizeData2(weu_dataPair *h, uint32_t dataSize) {
    if (h == NULL) return;
    h->dataSize2 = dataSize;
    h->data = realloc(h->data, h->dataSize1 + dataSize);
    memset(h->data + h->dataSize1, 0, dataSize);
}

#endif
#endif