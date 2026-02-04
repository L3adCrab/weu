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
/////////////////////////////////////////////////////////////////////////////////////
// EXAMPLE Create array and set and getting data
#include <stdio.h>

#define WEU_IMPLEMENTATION
#include "include/weu/weu_array.h"
#include "include/weu/weu_string.h"

int main() {
    //  allocate array
    weu_array *array = weu_array_new(15, sizeof(weu_string*), weu_string_listFree);

    //  push data to array
    for (uint32_t i = 0; i < array->capacity; i++)
    {
        weu_string *str = weu_string_int(i + 1);
        weu_array_push(array, &str);
    }

    // read data from array
    for (uint32_t i = 0; i < array->count; i++)
    {
        weu_string *str;
        weu_array_getAt(array, i, &str);
        if (str != NULL) printf("STR - %s\n", str->text);
    }
    
    //  free array
    weu_array_free(&array);
}
*////////////////////////////////////////////////////////////////////////////////////

#ifndef WEU_ARRAY_H
#define WEU_ARRAY_H

#define WEUDEF extern

#include "weu_datatypes.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////
//  ALLOCATION
/////////////////////////////////////////////////////////////////////////////////////////////////////

/*  Returns pointer to weu_array. Has to be freed using weu_array_free.

For data element deallocation can be used destructorFun callback.

@param capacity     Set maximum element count of array
@param sizeOfData   Set size of array element
@param destructorFun Used for freeing memory allocated for data, can be set to NULL
*/
WEUDEF weu_array *weu_array_new(uint32_t capacity, uint32_t sizeOfData, datafreefun destructorFun);

/*  Deallocates array

@param h        Reference to array pointer
@param freeData If set to true, if set calls array`s destructorFun
*/
WEUDEF void weu_array_free(weu_array **h, bool freeData);

/*  Reallocates array

@param h            Handle to array
@param newLength    New capacity of array
@param freeOutOfBounds   If set to true, if set calls destructorFun
*/
WEUDEF void weu_array_resize(weu_array *h, uint32_t newLength, bool freeOutOfBounds);

/*  Set array data destructor callback

@param h            Handle to array
@param destructorFun Used for freeing memory allocated for data, can be set to NULL
*/
WEUDEF void weu_array_setDestructor(weu_array *h, datafreefun destructorFun);
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  DATA
/////////////////////////////////////////////////////////////////////////////////////////////////////

/*  Get data at specified index.

If index out of array bounds out set to NULL.

@param h        Handle to array
@param index    Location of data
@param out      Reference pointer to output data
*/
WEUDEF void weu_array_getAt(weu_array *h, uint32_t index, void *out);

/*  Set data at specified index.

If index out of array bounds returns.

@param h        Handle to array
@param index    Location of data
@param data     Reference pointer to data
*/
WEUDEF void weu_array_setAt(weu_array *h, uint32_t index, void *data);

/*  Zeroes out data at specified index.

If index out of array bounds returns.

@param h        Handle to array
@param index    Location of data
@param freeData If set to true, if set calls array`s destructorFun
*/
WEUDEF void weu_array_clearAt(weu_array *h, uint32_t index, bool freeData);

/*
@param h    Handle to array
*/
WEUDEF bool weu_array_isEmpty(weu_array *h);

/*  Sets array memory to 0

@param h        Handle to array
@param freeData If set calls destructorFun
*/
WEUDEF void weu_array_empty(weu_array *h, bool freeData);
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  END
/////////////////////////////////////////////////////////////////////////////////////////////////////

/*  Insert data at the end of the array

@param h    Handle to array
@param data Reference pointer to data
*/
WEUDEF void weu_array_push(weu_array *h, void *data);

/*  Remove data from the end of the array

@param h        Handle to array
@param out      Reference pointer to output data
@param freeData If set calls destructorFun, out set to NULL
*/
WEUDEF void weu_array_pop(weu_array *h, void *out, bool freeData);
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  BEG
/////////////////////////////////////////////////////////////////////////////////////////////////////

/*  Insert data at front of the array

@param h    Handle to array
@param data Reference pointer to data
*/  
WEUDEF void weu_array_unshift(weu_array *h, void *data);

/*  Remove data at front of the array

@param h        Handle to array
@param out      Reference pointer to output data
@param freeData If set calls destructorFun, out set to NULL
*/  
WEUDEF void weu_array_shift(weu_array *h, void *out, bool freeData);

#ifdef WEU_IMPLEMENTATION

#include <stdlib.h>
#include <memory.h>

//  Set output to NULL
static void _weu_array_0(weu_array *h, void *out) {
    if (out != NULL) memset(out, 0, h->dataSize);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//  ALLOCATION
/////////////////////////////////////////////////////////////////////////////////////////////////////

weu_array *weu_array_new(uint32_t capacity, uint32_t sizeOfData, datafreefun destructorFun) {
    weu_array *out = (weu_array*)malloc(sizeof(weu_array));
    out->data = calloc(capacity, sizeOfData);
    if (out->data == NULL) {
        free(out);
        return NULL;
    }
    out->count      = 0;
    out->capacity   = capacity;
    out->dataSize   = sizeOfData;
    out->d          = destructorFun;
    return out;
}
void weu_array_free(weu_array **h, bool freeData) {
    if (*h == NULL) return;
    weu_array *arr = *h;
    if (freeData && arr->d != NULL) {
        for (uint32_t i = 0; i < arr->capacity; i++)
        {
            void *data;
            weu_array_getAt(arr, i, &data);
            arr->d(&data);
        }
    }
    free((*h)->data);
    free(*h);
    *h = NULL;
}
void weu_array_resize(weu_array *h, uint32_t newLength, bool freeOutOfBounds) {
    if (h == NULL) return;
    if (newLength > h->capacity) {
        h->data = realloc(h->data, h->dataSize * newLength);
        memset(h->data + (h->capacity * h->dataSize), 0, h->dataSize * (newLength - h->capacity));
        h->capacity = newLength;
    }
    else if (newLength < h->capacity && freeOutOfBounds && h->d != NULL) {
        for (uint32_t i = newLength; i < h->capacity; i++) {
            void *temp;
            weu_array_getAt(h, i, &temp);
            h->d(&temp);
        }
    } 
    h->count = newLength;
}

void weu_array_setDestructor(weu_array *h, datafreefun destructorFun) {
    if (h == NULL) return;
    h->d = destructorFun;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  DATA
/////////////////////////////////////////////////////////////////////////////////////////////////////

void weu_array_getAt(weu_array *h, uint32_t index, void *out) {
    if (h == NULL || out == NULL)           { _weu_array_0(h, out); return; }
    if (h->count == 0 || index >= h->count) { _weu_array_0(h, out); return; }
    memmove(out, h->data + (index * h->dataSize), h->dataSize);
}
void weu_array_setAt(weu_array *h, uint32_t index, void *data) {
    if (h == NULL) return;
    if (index >= h->count) return;
    memmove(h->data + (h->dataSize * index), data, h->dataSize);
}
void weu_array_clearAt(weu_array *h, uint32_t index, bool freeData) {
    if (h == NULL) return;
    if (index >= h->count) return;
    if (freeData && h->d != NULL) {
        void *data;
        weu_array_getAt(h, index, &data);
        h->d(&data);
    }
    memset(h->data + (h->dataSize * index), 0, h->dataSize);
}

bool weu_array_isEmpty(weu_array *h) {
    if (h == NULL) return true;
    if (h->count == 0) return true;
    return false;
}
void weu_array_empty(weu_array *h, bool freeData) {
    if (h == NULL) return;
    if (freeData && h->d != NULL) {
        for (uint32_t i = 0; i < h->capacity; i++)
        {
            void *data;
            weu_array_getAt(h, i, &data);
            h->d(&data);
        }
    }
    memset(h->data, 0, h->dataSize * h->capacity);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  BEG
/////////////////////////////////////////////////////////////////////////////////////////////////////

void weu_array_unshift(weu_array *h, void *data) {
    if (h == NULL) return;
    uint32_t oldLen = h->count;
    weu_array_resize(h, oldLen + 1, false);
    memmove(h->data + h->dataSize, h->data, h->dataSize * oldLen);
    memcpy(h->data, data, h->dataSize);
}
void weu_array_shift(weu_array *h, void *out, bool freeData) {
    if (h == NULL)      { _weu_array_0(h, out); return; }
    if (h->count == 0)  { _weu_array_0(h, out); return; }
    if (freeData && h->d != NULL) {
        void *temp;
        weu_array_getAt(h, 0, &temp);
        h->d(&temp);
        _weu_array_0(h, out);
    }
    else if (out != NULL) memcpy(out, h->data, h->dataSize);
    memmove(h->data, h->data + h->dataSize, (h->count - 1) * h->dataSize);
    weu_array_resize(h, h->count - 1, false);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  END
/////////////////////////////////////////////////////////////////////////////////////////////////////

void weu_array_push(weu_array *h, void *data) {
    if (h == NULL) return;
    weu_array_resize(h, h->count + 1, false);
    weu_array_setAt(h, h->count - 1, data);
}
void weu_array_pop(weu_array *h, void *out, bool freeData) {
    if (h == NULL)      { _weu_array_0(h, out); return; }
    if (h->count == 0)  { _weu_array_0(h, out); return; }
    if (freeData) {
        _weu_array_0(h, out);
    }
    else if (out != NULL) memcpy(out, h->data + ((h->count - 1) * h->dataSize), h->dataSize);
    weu_array_resize(h, h->count - 1, freeData);
}

#endif
#endif