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

#ifndef weu_list_h
#define weu_list_h

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
#include <memory.h>
#include <stdio.h>

/////////////////////////////////////////////////////////////////////////////////////////////////////
//  ALLOCATION

WEUDEF weu_list *weu_list_new(unsigned int size, datafreefun d);
WEUDEF void weu_list_free(weu_list **h);
WEUDEF void weu_list_resize(weu_list *h, int size, bool freeOOB);
WEUDEF void weu_list_freeData(weu_list *h, int index);
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  FREE

WEUDEF void weu_stdFree(void **data);
WEUDEF void weu_list_setFreeFun(weu_list *h, datafreefun d);
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  DATA

WEUDEF void weu_list_setData(weu_list *h, int index, void *data);
WEUDEF void *weu_list_getData(weu_list *h, int index);

WEUDEF void weu_list_insertData(weu_list *h, int index, void *data);
WEUDEF void *weu_list_removeData(weu_list *h, int index, bool free);
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  BEG

//  Insert data at front of the weu_list
WEUDEF void weu_list_unshift(weu_list *h, void *data);
//  Remove data at front of the weu_list
//  If free is set to 0/FALSE returns pointer to data
WEUDEF void *weu_list_shift(weu_list *h, bool free);
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  END

//  Insert data at the end of the weu_list
WEUDEF void weu_list_push(weu_list *h, void *data);
//  Remove data from the end of the weu_list
//  If free is set to 0/FALSE returns pointer to data
WEUDEF void *weu_list_pop(weu_list *h, bool free);
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  FROM TO

//  Frees data in range of index's from(inclusive) - to(exclusive)
//  [from - to) 
WEUDEF void weu_list_removeFromTo(weu_list *h, int from, int to, bool free);
//  Inserts empty slots in range of from - to
WEUDEF void weu_list_spaceFromTo(weu_list *h, int from, int to);

#ifdef WEU_IMPLEMENTATION

/////////////////////////////////////////////////////////////////////////////////////////////////////
//  ALLOCATION
/////////////////////////////////////////////////////////////////////////////////////////////////////

weu_list *weu_list_new(unsigned int size, datafreefun d) {
    weu_list *out = (weu_list*)malloc(sizeof(weu_list));
    if (!out) return NULL;
    out->length     = size;
    out->data       = (void**)calloc(size, sizeof(void*));
    if (!out->data) return NULL;
    out->d          = d;
    return out;
}
void weu_list_free(weu_list **h) {
    if (*h == NULL) return;
    for (int i = 0; i < (*h)->length; i++) {
        weu_list_freeData(*h, i);
    }
    free((*h)->data);
    free(*h);
    *h = NULL;
}
void weu_list_resize(weu_list *h, int size, bool freeOOB) {
    if (h == NULL) return;
    size = size > 0 ? size : 0;
    if (freeOOB) {
        for (int i = size; i < h->length; i++) {
            weu_list_freeData(h, i);
        }
    }
    int oldLen = h->length;
    h->length       = size;
    h->data         = (void**)realloc(h->data, sizeof(void*) * size);
    if (size > oldLen) {
        memset(h->data + oldLen, 0, sizeof(void*) * (size - oldLen));
    }
}
//  Free data from weu_list and set to NULL at specified index without resizing weu_list
void weu_list_freeData(weu_list *h, int index) {
    if (h == NULL || index < 0 || index >= h->length) return;
    if (h->d != NULL) h->d(&h->data[index]);
    h->data[index] = NULL;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  FREE
/////////////////////////////////////////////////////////////////////////////////////////////////////

void weu_stdFree(void **data) {
    if (*data == NULL) return;
    free(*data);
    *data = NULL;
}
void weu_list_setFreeFun(weu_list *h, datafreefun d) {
    if (h == NULL) return;
    h->d = d;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  DATA
/////////////////////////////////////////////////////////////////////////////////////////////////////

void weu_list_setData(weu_list *h, int index, void *data) {
    if (h == NULL || index < 0 || index >= h->length) return;
    h->data[index]         = data;
}
void *weu_list_getData(weu_list *h, int index) {
    if (h == NULL || index < 0 || index >= h->length) return NULL;
    return h->data[index];
}

void weu_list_insertData(weu_list *h, int index, void *data) {
    if (h == NULL || index < 0 || index >= h->length) return;
    weu_list_resize(h, h->length + 1, 0);
    for (int i = h->length - 1; i > index; i--) {
        h->data[i]      = h->data[i - 1];
    }
    h->data[index]      = data;
}
//  If free is set to 0/FALSE returns pointer to data
void *weu_list_removeData(weu_list *h, int index, bool free) {
    if (h == NULL || index < 0 || index >= h->length) return NULL;
    void *out = NULL;
    if (!free) {
        out = h->data[index];
    }
    else {
        weu_list_freeData(h, 0);
        out = NULL;
    }
    for (int i = index; i < h->length - 1; i++) {
        h->data[i] = h->data[i + 1];
    }
    weu_list_resize(h, h->length - 1, false);
    return out;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  BEG
/////////////////////////////////////////////////////////////////////////////////////////////////////

//  Insert data at front of the weu_list
void weu_list_unshift(weu_list *h, void *data) {
    if (h == NULL) return;
    weu_list_resize(h, h->length + 1, false);
    for (int i = h->length - 1; i > 0; i--) {
        h->data[i]      = h->data[i - 1];
    }
    h->data[0]          = data;
}
//  Remove data at front of the weu_list
//  If free is set to 0/FALSE returns pointer to data
void *weu_list_shift(weu_list *h, bool free) {
    if (h == NULL || h->length <= 0) return NULL;
    void *out;
    if (!free) {
        out = h->data[0];
    }
    else {
        weu_list_freeData(h, 0);
        out = NULL;
    }
    for (int i = 0; i < h->length - 1; i++) {
        h->data[i]      = h->data[i + 1];
    }
    weu_list_resize(h, h->length - 1, false);
    return out;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  END
/////////////////////////////////////////////////////////////////////////////////////////////////////

//  Insert data at the end of the weu_list
void weu_list_push(weu_list *h, void *data) {
    if (h == NULL) return;
    weu_list_resize(h, h->length + 1, 0);
    h->data[h->length - 1]          = data;
}
//  Remove data from the end of the weu_list
//  If free is set to 0/FALSE returns pointer to data
void *weu_list_pop(weu_list *h, bool free) {
    if (h == NULL || h->length <= 0) return NULL;
    void *out;
    if (!free) {
        out = h->data[h->length - 1];
    }
    else {
        weu_list_freeData(h, 0);
        out = NULL;
    }
    weu_list_resize(h, h->length - 1, false);
    return out;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  FROM TO
/////////////////////////////////////////////////////////////////////////////////////////////////////

//  Frees data in range of index's from(inclusive) - to(exclusive)
//  [from - to) 
void weu_list_removeFromTo(weu_list *h, int from, int to, bool free) {
    if (h == NULL || from < 0 || to > h->length) return;
    int diff = to - from;
    if (diff < 0) return;
    for (int i = 0; i < diff; i++) {
        if (free) weu_list_freeData(h, from + i);
        else h->data[from + i] = NULL;
    }
    for (int i = to; i < h->length; i++) {
        h->data[i - diff] = h->data[i]; 
    }
    weu_list_resize(h, h->length - diff, false);
}
//  Inserts empty slots in range of from - to
void weu_list_spaceFromTo(weu_list *h, int from, int to) {
    if (h == NULL || from < 0) return;
    int diff = to - from;
    if (diff < 0) return;
    weu_list_resize(h, h->length + diff, false);
    for (int i = h->length - 1; i >= to; i--) {
        h->data[i] = h->data[i - diff];
    }
    memset(h->data + from, 0, diff * sizeof(void*));
}

#endif
#endif