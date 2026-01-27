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
#include <stdbool.h>

/////////////////////////////////////////////////////////////////////////////////////////////////////
//  ALLOCATION

WEUDEF weu_list *weu_list_new(uint32_t length, datafreefun d);
WEUDEF void weu_list_free(weu_list **h);
WEUDEF void weu_list_resize(weu_list *h, uint32_t newLength, bool freeOOB);
WEUDEF void weu_list_reallocateToLength(weu_list *h);
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  DATA FREE

WEUDEF void weu_list_freeData(weu_list *h, uint32_t index);
WEUDEF void weu_list_freeAllData(weu_list *h);
WEUDEF void weu_list_setDataFreeFun(weu_list *h, datafreefun freeFun);
WEUDEF void weu_stdFree(void **data);
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  DATA

WEUDEF void *weu_list_getData(weu_list *h, uint32_t index);
WEUDEF void weu_list_setData(weu_list *h, uint32_t index, void *data);

WEUDEF void weu_list_insertData(weu_list *h, uint32_t index, void *data);
WEUDEF void *weu_list_removeData(weu_list *h, uint32_t index, bool freeData);
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  CONTAINS

WEUDEF bool weu_list_containsObj(weu_list *h, void *obj, uint32_t *outIndex);
WEUDEF bool weu_list_containsValue(weu_list *h, void *value, uint32_t *outIndex, datacompfun compareFun);
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  BEG

//  Insert data at front of the list
WEUDEF void weu_list_unshift(weu_list *h, void *data);
//  Remove data at front of the list
//  If free is set to 0/FALSE returns pointer to data
WEUDEF void *weu_list_shift(weu_list *h, bool freeData);
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  END

//  Insert data at the end of the list
WEUDEF void weu_list_push(weu_list *h, void *data);
//  Remove data from the end of the list
//  If free is set to 0/FALSE returns pointer to data
WEUDEF void *weu_list_pop(weu_list *h, bool freeData);
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  FROM TO

//  Frees data in range of index's from(inclusive) - to(exclusive)
//  [from - to) 
WEUDEF void weu_list_removeFromTo(weu_list *h, uint32_t from, uint32_t to, bool freeData);
WEUDEF void weu_list_removeAt(weu_list *h, uint32_t index, uint32_t count, bool freeData);
//  Inserts empty slots in range of from - to
WEUDEF void weu_list_spaceFromTo(weu_list *h, uint32_t from, uint32_t to);
WEUDEF void weu_list_spaceAt(weu_list *h, uint32_t index, uint32_t count);

#ifdef WEU_IMPLEMENTATION

/////////////////////////////////////////////////////////////////////////////////////////////////////
//  ALLOCATION

weu_list *weu_list_new(uint32_t length, datafreefun d) {
    weu_list *out = (weu_list*)malloc(sizeof(weu_list));
    out->length = length;
    out->allocatedLength = length;
    out->data = calloc(length, sizeof(void*));
    out->d = d;
    return out;
}
void weu_list_free(weu_list **h) {
    if (*h == NULL) return;
    free((*h)->data);
    free(*h);
    *h = NULL;
}
void weu_list_resize(weu_list *h, uint32_t newLength, bool freeOOB) {
    if (h == NULL) return;
    uint32_t oldLen = h->length;
    int lenDif = newLength - oldLen;
    if (lenDif > 0) {
        h->length = newLength;
        if (newLength > h->allocatedLength) {
            h->allocatedLength = newLength;
            h->data = realloc(h->data, sizeof(void*) * newLength);
        }
    }
    else {
        h->length = newLength;
        if (freeOOB) {
            for (uint32_t i = newLength; i < oldLen; i++)
            {
                weu_list_freeData(h, i);
            }
        }
    }
}
void weu_list_reallocateToLength(weu_list *h) {
    if (h == NULL) return;
    h->allocatedLength = h->length;
    h->data = realloc(h->data, h->allocatedLength * sizeof(void*));
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  DATA FREE

void weu_list_freeData(weu_list *h, uint32_t index) {
    if (h == NULL || index >= h->length) return;
    if (h->d != NULL) h->d(&h->data[index]);
    h->data[index] = NULL;
}
void weu_list_freeAllData(weu_list *h) {
    if (h == NULL) return;
    for (uint32_t i = 0; i < h->length; i++) {   
        weu_list_freeData(h, i);
    }
    h->length = 0;
}
void weu_list_setDataFreeFun(weu_list *h, datafreefun freeFun) {
    if (h == NULL) return;
    h->d = freeFun;
}
void weu_stdFree(void **data) {
    free(*data);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  DATA

void *weu_list_getData(weu_list *h, uint32_t index) {
    if (h == NULL || index >= h->length) return NULL;
    return h->data[index];
}
void weu_list_setData(weu_list *h, uint32_t index, void *data) {
    if (h == NULL || index >= h->length) return;
    h->data[index] = data;
}

void weu_list_insertData(weu_list *h, uint32_t index, void *data) {
    if (h == NULL || index > h->length) return;
    weu_list_resize(h, h->length + 1, false);
    for (uint32_t i = h->length - 1; i > index; i--) {
        h->data[i] = h->data[i - 1];
    }
    h->data[index] = data;
}
void *weu_list_removeData(weu_list *h, uint32_t index, bool freeData) {
    if (h == NULL || index >= h->length) return NULL;
    void *out = h->data[index];
    if (freeData) {
        if (h->d != NULL) h->d(&out);
    }
    for (uint32_t i = index; i < h->length - 1; i++)
    {
        h->data[i] = h->data[i + 1];
    }
    weu_list_resize(h, h->length - 1, false);
    return out;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  CONTAINS

bool weu_list_containsObj(weu_list *h, void *obj, uint32_t *outIndex) {
    *outIndex = 0;
    if (h == NULL) return false;
    for (uint32_t i = 0; i < h->length; i++)
    {
        if (h->data[i] == obj) {
            *outIndex = i;
            return true;
        }
    }
    return false;
}
bool weu_list_containsValue(weu_list *h, void *value, uint32_t *outIndex, datacompfun compareFun) {
    *outIndex = 0;
    if (h == NULL) return false;
    for (uint32_t i = 0; i < h->length; i++)
    {
        if (compareFun(h->data[i], value)) {
            *outIndex = i;
            return true;
        }
    }
    return false;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  BEG

//  Insert data at front of the list
void weu_list_unshift(weu_list *h, void *data) {
    if (h == NULL) return;
    h->length++;
    weu_list_resize(h, h->length, false);
    for (int i = h->length - 1; i > 0; i--)
    {
        h->data[i] = h->data[i - 1];
    }
    h->data[0] = data;
}
//  Remove data at front of the list
//  If free is set to 0/FALSE returns pointer to data
void *weu_list_shift(weu_list *h, bool freeData) {
    if (h == NULL || h->length == 0) return NULL;
    void *out = h->data[0];
    if (freeData) {
        weu_list_freeData(h, 0);
    }
    for (uint32_t i = 0; i < h->length - 1; i++)
    {
        h->data[i] = h->data[i + 1];
    }
    h->data[h->length - 1] = NULL;
    h->length--;
    return out;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  END

//  Insert data at the end of the list
void weu_list_push(weu_list *h, void *data) {
    if (h == NULL) return;
    weu_list_resize(h, h->length + 1, 0);
    h->data[h->length - 1] = data;
}
//  Remove data from the end of the list
//  If free is set to 0/FALSE returns pointer to data
void *weu_list_pop(weu_list *h, bool freeData) {
    if (h == NULL || h->length == 0) return NULL;
    void *out = h->data[h->length - 1];
    if (freeData) {
        weu_list_freeData(h, h->length - 1);
    }
    h->length--;
    return out;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  FROM TO

//  Frees data in range of index's from(inclusive) - to(exclusive)
//  [from - to) 
void weu_list_removeFromTo(weu_list *h, uint32_t from, uint32_t to, bool freeData) {
    if (h == NULL || from > to || to > h->length) return;
    uint32_t dif = to - from;
    for (uint32_t i = from; i < to; i++)
    {
        if (freeData) {
            weu_list_freeData(h, i);
        }
    }
    for (uint32_t i = to; i < h->length; i++)
    {
        h->data[i - dif] = h->data[i];
    }
    weu_list_resize(h, h->length - dif, false);
}
void weu_list_removeAt(weu_list *h, uint32_t index, uint32_t count, bool freeData) {
    weu_list_removeFromTo(h, index, index + count, freeData);
}
//  Inserts empty slots in range of from - to
void weu_list_spaceFromTo(weu_list *h, uint32_t from, uint32_t to) {
    if (h == NULL || from > to) return;
    uint32_t dif = to - from;
    uint32_t oldLen = h->length;
    weu_list_resize(h, h->length + dif, false);
    for (uint32_t i = oldLen; i >= from; i--)
    {
        h->data[i + dif] = h->data[i];
    }
    for (uint32_t i = from; i < to; i++)
    {
        h->data[i] = NULL;
    }
}
void weu_list_spaceAt(weu_list *h, uint32_t index, uint32_t count) {
    weu_list_spaceFromTo(h, index, index + count);
}

#endif
#endif