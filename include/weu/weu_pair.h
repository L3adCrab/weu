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
    weu_pair    *pair = weu_pair_new(sizeof(data1), sizeof(data2));
    weu_pair_setData(pair, &data1, &data2);

    //  get data
    uint32_t        out1;
    weu_string      *out2;
    weu_pair_getData(pair, &out1, &out2);
    printf("DATA RETRIEVED FROM PAIR\n");
    printf("int - %i | str - %s\n", out1, out2->text);

    //  free string and pair
    weu_string_free(&out2);
    weu_pair_free(&pair);
}
*////////////////////////////////////////////////////////////////////////////////////

#ifndef WEU_PAIR_H
#define WEU_PAIR_H

#define WEUDEF extern

#include "weu_datatypes.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////
//  ALLOCATION

WEUDEF weu_pair *weu_pair_new(uint32_t dataSize_1, uint32_t dataSize_2, datafreefun destructor_1, datafreefun destructor_2);
WEUDEF void weu_pair_free(weu_pair **h);

WEUDEF void weu_pair_setDestructor1(weu_pair *h, datafreefun destructor_1);
WEUDEF void weu_pair_setDestructor2(weu_pair *h, datafreefun destructor_2);
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  GET DATA

WEUDEF void weu_pair_getData(weu_pair *h, void *out1, void *out2);
WEUDEF void weu_pair_getData1(weu_pair *h, void *out);
WEUDEF void weu_pair_getData2(weu_pair *h, void *out);
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  SET DATA

WEUDEF void weu_pair_setData(weu_pair *h, void *data1, void *data2);
WEUDEF void weu_pair_setData1(weu_pair *h, void *data);
WEUDEF void weu_pair_setData2(weu_pair *h, void *data);

#ifdef WEU_IMPLEMENTATION

#include <stdlib.h>
#include <stdio.h>
#include <memory.h>

/////////////////////////////////////////////////////////////////////////////////////////////////////
//  ALLOCATION
/////////////////////////////////////////////////////////////////////////////////////////////////////

weu_pair *weu_pair_new(uint32_t dataSize_1, uint32_t dataSize_2, datafreefun destructor_1, datafreefun destructor_2) {
    weu_pair *out = (weu_pair*)malloc(sizeof(weu_pair));
    out->dataSize1 = dataSize_1;
    out->dataSize2 = dataSize_2;
    out->data = calloc(1, dataSize_1 + dataSize_2);
    out->d1 = destructor_1;
    out->d2 = destructor_2;
    return out;
}
void weu_pair_free(weu_pair **h) {
    if (*h == NULL) return;
    weu_pair *pair = *h;
    if (pair->d1)   pair->d1(pair->data);
    if (pair->d2)   pair->d2(pair->data + pair->dataSize1);
    free((*h)->data);
    free(*h);
    *h = NULL;
}

void weu_pair_setDestructor1(weu_pair *h, datafreefun destructor_1) {
    if (!h) return;
    h->d1 = destructor_1;
}
void weu_pair_setDestructor2(weu_pair *h, datafreefun destructor_2) {
    if (!h) return;
    h->d2 = destructor_2;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  GET DATA
/////////////////////////////////////////////////////////////////////////////////////////////////////

void weu_pair_getData(weu_pair *h, void *out1, void *out2) {
    if (h == NULL) { printf("pair = null\n"); out1 = NULL; out2 = NULL; return; }
    memmove(out1, h->data, h->dataSize1);
    memmove(out2, h->data + h->dataSize1, h->dataSize2);
}
void weu_pair_getData1(weu_pair *h, void *out) {
    if (h == NULL) { out = NULL; return; }
    memmove(out, h->data, h->dataSize1);
}
void weu_pair_getData2(weu_pair *h, void *out) {
    if (h == NULL) { out = NULL; return; }
    memmove(out, h->data + h->dataSize1, h->dataSize2);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  SET DATA
/////////////////////////////////////////////////////////////////////////////////////////////////////

void weu_pair_setData(weu_pair *h, void *data1, void *data2) {
    if (h == NULL) return;
    weu_pair_setData1(h, data1);
    weu_pair_setData2(h, data2);
}
void weu_pair_setData1(weu_pair *h, void *data) {
    if (h == NULL) return;
    if (data == NULL)   memset(h->data, 0, h->dataSize1);
    else                memmove(h->data, data, h->dataSize1);
}
void weu_pair_setData2(weu_pair *h, void *data) {
    if (h == NULL) return;
    if (data == NULL)   memset(h->data + h->dataSize1, 0, h->dataSize2);
    else                memmove(h->data + h->dataSize1, data, h->dataSize2);
}

#endif
#endif