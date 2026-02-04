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
// EXAMPLE Create dataPair and set and getting data
#include <stdio.h>

#define WEU_IMPLEMENTATION
#include "include/weu/weu_datapair.h"
#include "include/weu/weu_string.h"

int main() {
    // allocate data
    uint32_t        data1 = 47;
    weu_string      *data2 = weu_string_new("test string");
    printf("DATA TO BE SAVED\n");
    printf("int - %i | str - %s\n", data1, data2->text);
    
    //  allocate pair and set data
    weu_dataPair    *pair = weu_dataPair_new(sizeof(data1), sizeof(data2));
    weu_dataPair_setData(pair, &data1, &data2);

    //  get data
    uint32_t        out1;
    weu_string      *out2;
    weu_dataPair_getData(pair, &out1, &out2);
    printf("DATA RETRIEVED FROM PAIR\n");
    printf("int - %i | str - %s\n", out1, out2->text);

    //  free string and pair
    weu_string_free(&out2);
    weu_dataPair_free(&pair);
}
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
/////////////////////////////////////////////////////////////////////////////////////////////////////

void weu_dataPair_getData(weu_dataPair *h, void *out1, void *out2) {
    if (h == NULL) { printf("pair = null\n"); out1 = NULL; out2 = NULL; return; }
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
/////////////////////////////////////////////////////////////////////////////////////////////////////

void weu_dataPair_setData(weu_dataPair *h, void *data1, void *data2) {
    if (h == NULL) return;
    weu_dataPair_setData1(h, data1);
    weu_dataPair_setData2(h, data2);
}
void weu_dataPair_setData1(weu_dataPair *h, void *data) {
    if (h == NULL) return;
    if (data == NULL)   memset(h->data, 0, h->dataSize1);
    else                memcpy(h->data, data, h->dataSize1);
}
void weu_dataPair_setData2(weu_dataPair *h, void *data) {
    if (h == NULL) return;
    if (data == NULL)   memset(h->data + h->dataSize1, 0, h->dataSize2);
    else                memcpy(h->data + h->dataSize1, data, h->dataSize2);
}

#endif
#endif