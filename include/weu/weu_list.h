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
#include "include/weu/weu_list.h"
#include "include/weu/weu_string.h"

bool containsString(void *data1, void *data2);

int main() {
    //  allocate array
    weu_list *array = weu_list_new(15, sizeof(weu_string*), weu_string_datafreefun);

    //  push data to array
    for (uint32_t i = 0; i < array->capacity; i++) 
    {
        weu_string *str = weu_string_int(i + 1);
        weu_list_push(array, &str);
    }

    weu_list_removeCount(array, 3, 5, true);

    // read data from array
    printf("COUNT - %i\n", array->count);
    uint32_t itterCount = array->count;
    for (uint32_t i = 0; i < itterCount; i++)
    {
        weu_string *str;
        weu_list_getAt(array, i, &str);

        printf ("%p - ", str);
        printf ("%i - ", i);
        if (str)    printf("STR - %s\n", str->text);
        else        printf("STR - NULL\n");
    }

    //  check if contains string 10
    uint32_t dataIndex = 0;
    weu_stringNA strCompareTo = weu_stringNA_new("10");
    if (weu_list_containsValue(array, &strCompareTo, &dataIndex, containsString)) 
            printf("Contains string %s at %i\n", strCompareTo.text, dataIndex);
    else    printf("Doesn`t contain string %s\n", strCompareTo.text);

    //  check if contains string 12
    strCompareTo = weu_stringNA_new("12");
    if (weu_list_containsValue(array, &strCompareTo, &dataIndex, containsString)) 
            printf("Contains string %s at %i\n", strCompareTo.text, dataIndex);
    else    printf("Doesn`t contain string %s\n", strCompareTo.text);
    
    //  free array
    weu_list_free(&array, true);
}

bool containsString(void *data1, void *data2) {
    if (!data1 || !data2) return false;
    weu_string *str1    = data1;
    weu_stringNA *str2  = data2;
    return weu_string_textMatches(str1->text, str2->text);
}
*////////////////////////////////////////////////////////////////////////////////////

#ifndef WEU_LIST_H
#define WEU_LIST_H

#define WEUDEF extern

#include "weu_datatypes.h"

#define WEU_LIST_INDEX_INVALID 0xffffffff

/////////////////////////////////////////////////////////////////////////////////////////////////////
//  ALLOCATION

/*  Returns pointer to weu_list. Has to be freed using weu_list_free.

For data element deallocation can be used destructorFun callback.

@param capacity     Set maximum element count of array
@param sizeOfData   Set size of array element
@param destructorFun Used for freeing memory allocated for data, can be set to NULL
*/
WEUDEF weu_list *weu_list_new(uint32_t capacity, uint32_t sizeOfData, datafreefun destructorFun);
/*
@param h Reference to array pointer
@param freeData If set calls destructorfun
*/
WEUDEF void weu_list_free(weu_list **h, bool freeData);
/*
@param h Handle to array
@param newLength
@param freeData If set calls destructorfun on data out of new bounds
*/
WEUDEF void weu_list_resize(weu_list *h, uint32_t newLength, bool freeData);

/*  Set array data destructor callback

@param h            Handle to array
@param destructorFun Used for freeing memory allocated for data, can be set to NULL
*/
WEUDEF void weu_list_setDestructor(weu_list *h, datafreefun destructorFun);
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  DATA

WEUDEF void weu_list_getAt(weu_list *h, uint32_t index, void *out);
WEUDEF void weu_list_setAt(weu_list *h, uint32_t index, void *data);
WEUDEF void weu_list_clearAt(weu_list *h, uint32_t index, bool freeData);

WEUDEF bool weu_list_isEmpty(weu_list *h);
//  If set, calls datafreefun
WEUDEF void weu_list_empty(weu_list *h);
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  CONTAINS

//  On fail or not found if indexOut not NULL, sets it to WEU_INDEX_INVALID
WEUDEF bool weu_list_containsPtr(weu_list *h, void *ptr, uint32_t *indexOut);
//  Returns output of datacompfun
//  On fail or not found if indexOut not NULL, sets it to WEU_INDEX_INVALID
WEUDEF bool weu_list_containsValue(weu_list *h, void *value, uint32_t *indexOut, datacompfun compareFun);
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  BEG

//  Insert data at front of the array
WEUDEF void weu_list_unshift(weu_list *h, void *data);
//  Remove data at front of the array
//  If freeData and datafreefun set out is NULL
WEUDEF void weu_list_shift(weu_list *h, void *out, bool freeData);
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  END

//  Insert data at the end of the array
WEUDEF void weu_list_push(weu_list *h, void *data);
//  Remove data from the end of the array
//  If freeData and datafreefun set out is NULL
WEUDEF void weu_list_pop(weu_list *h, void *out, bool freeData);
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  FROM TO
/////////////////////////////////////////////////////////////////////////////////////////////////////

/*
@param index inclusive
*/
WEUDEF void weu_list_insertCount(weu_list *h, uint32_t index, uint32_t count);
/*
@param from inclusive
@param to   exclusive
*/
WEUDEF void weu_list_insertFromTo(weu_list *h, uint32_t from, uint32_t to);

WEUDEF void weu_list_removeCount(weu_list *h, uint32_t index, uint32_t count, bool freeData);
WEUDEF void weu_list_removeFromTo(weu_list *h, uint32_t from, uint32_t to, bool freeData);


#ifdef WEU_IMPLEMENTATION

#include <stdlib.h>
#include <memory.h>

//  Set output to NULL
static void _weu_list_0(weu_list *h, void *out) {
    if (out != NULL) memset(out, 0, h->dataSize);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//  ALLOCATION
/////////////////////////////////////////////////////////////////////////////////////////////////////

weu_list *weu_list_new(uint32_t capacity, uint32_t sizeOfData, datafreefun destructorFun) {
    weu_list *out = (weu_list*)malloc(sizeof(weu_list));
    if (!out) return NULL;
    out->data = calloc(capacity, sizeOfData);
    if (!out->data) {
        free(out);
        return NULL;
    }
    out->count      = 0;
    out->capacity   = capacity;
    out->dataSize   = sizeOfData;
    out->d          = destructorFun;
    return out;
}
void weu_list_free(weu_list **h, bool freeData) {
    if (!*h) return;
    weu_list *arr = *h;
    if (freeData && arr->d) {
        for (uint32_t i = 0; i < arr->count; i++)
        {
            arr->d(arr->data + (i * arr->dataSize));
        }
    }
    memset(arr->data, 0, (arr->capacity) * arr->dataSize);
    free((*h)->data);
    free(*h);
    *h = NULL;
}
void weu_list_resize(weu_list *h, uint32_t newLength, bool freeData) {
    if (!h) return;
    if (newLength > h->capacity) {
        h->data = realloc(h->data, h->dataSize * newLength);
        memset(h->data + (h->capacity * h->dataSize), 0, h->dataSize * (newLength - h->capacity));
        h->capacity = newLength;
    }
    else if (freeData && h->d && newLength < h->count) {
        for (uint32_t i = newLength; i < h->capacity; i++)
        {
            h->d(h->data + (i * h->dataSize));
        }
        memset(h->data + (newLength * h->dataSize) , 0, (h->capacity - newLength) * h->dataSize);
    }
    h->count = newLength;
}

void weu_list_setDestructor(weu_list *h, datafreefun destructorFun) {
    if (h == NULL) return;
    h->d = destructorFun;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  DATA
/////////////////////////////////////////////////////////////////////////////////////////////////////

void weu_list_getAt(weu_list *h, uint32_t index, void *out) {
    if (!h || !out)                     { _weu_list_0(h, out); return; }
    if (!h->count || index >= h->count) { _weu_list_0(h, out); return; }
    memmove(out, h->data + (index * h->dataSize), h->dataSize);
}
void weu_list_setAt(weu_list *h, uint32_t index, void *data) {
    if (!h) return;
    if (index >= h->count) return;
    memmove(h->data + (h->dataSize * index), data, h->dataSize);
}
void weu_list_clearAt(weu_list *h, uint32_t index, bool freeData) {
    if (h == NULL) return;
    if (index >= h->count) return;
    if (freeData && h->d) {
        h->d(h->data + (index * h->dataSize));
    }
    memset(h->data + (h->dataSize * index), 0, h->dataSize);
}

bool weu_list_isEmpty(weu_list *h) {
    if (!h) return true;
    return h->count == 0;
}
void weu_list_empty(weu_list *h) {
    if (!h) return;
    if (h->d) {
        for (uint32_t i = 0; i < h->capacity; i++)
        {
            h->d(h->data + (i * h->dataSize));
        }
    }
    memset(h->data, 0, h->dataSize * h->capacity);
}
void weu_list_clearCount(weu_list *h, uint32_t index, uint32_t count, bool freeData) {
    if (!h || !count)       return;
    if (index >= h->count)  return;
    if (index + count > h->count) count = index + count - h->count;
    if (freeData && h->d) {
        for (uint32_t i = index; i < index + count; i++)
        {
            h->d(h->data + (i * h->dataSize));
        }
    }
    memset(h->data + (index * h->dataSize), 0, count * h->dataSize);
}
void weu_list_clearFromTo(weu_list *h, uint32_t from, uint32_t to, bool freeData) {
    if (!h || from > to) return;
    if (to > h->count) to = h->count;
    int32_t count = to - from;
    if (count < 0) return;
    weu_list_clearCount(h, from, count, freeData);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  CONTAINS
/////////////////////////////////////////////////////////////////////////////////////////////////////

bool weu_list_containsPtr(weu_list *h, void *ptr, uint32_t *indexOut) {
    if (!h) {
        if (indexOut != NULL) *indexOut = WEU_INDEX_INVALID;
        return false;
    }
    for (uint32_t i = 0; i < h->count; i++)
    {
        if (h->data + (i * h->dataSize) == ptr) {
            if (indexOut != NULL) *indexOut = i;
            return true;
        }
    }
    if (indexOut != NULL) *indexOut = WEU_INDEX_INVALID;
    return false;
}
bool weu_list_containsValue(weu_list *h, void *value, uint32_t *indexOut, datacompfun compareFun) {
    if (!h || !compareFun) {
        if (indexOut) (*indexOut = WEU_INDEX_INVALID);
        return false;
    }
    for (uint32_t i = 0; i < h->count; i++)
    {
        if (compareFun(h->data + (i * h->dataSize), value)) {
            if (indexOut) *indexOut = i;
            return true;
        }
    }
    if (indexOut) *indexOut = WEU_INDEX_INVALID;
    return false;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  BEG
/////////////////////////////////////////////////////////////////////////////////////////////////////

void weu_list_unshift(weu_list *h, void *data) {
    if (!h) return;
    uint32_t oldLen = h->count;
    weu_list_resize(h, oldLen + 1, false);
    memmove(h->data + h->dataSize, h->data, h->dataSize * oldLen);
    memcpy(h->data, data, h->dataSize);
}
void weu_list_shift(weu_list *h, void *out, bool freeData) {
    if (!h)      { _weu_list_0(h, out); return; }
    if (h->count == 0)  { _weu_list_0(h, out); return; }
    if (freeData && h->d != NULL) {
        h->d(h->data);
        _weu_list_0(h, out);
    }
    else if (out != NULL) memcpy(out, h->data, h->dataSize);
    memmove(h->data, h->data + h->dataSize, (h->count - 1) * h->dataSize);
    weu_list_resize(h, h->count - 1, false);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  END
/////////////////////////////////////////////////////////////////////////////////////////////////////

void weu_list_push(weu_list *h, void *data) {
    if (!h) return;
    weu_list_resize(h, h->count + 1, false);
    weu_list_setAt(h, h->count - 1, data);
}
void weu_list_pop(weu_list *h, void *out, bool freeData) {
    if (!h)             { _weu_list_0(h, out); return; }
    if (h->count == 0)  { _weu_list_0(h, out); return; }
    if (freeData) {
        h->d(h->data + ((h->count - 1) * h->dataSize));
        _weu_list_0(h, out);
    }
    else if (out != NULL) memcpy(out, h->data + ((h->count - 1) * h->dataSize), h->dataSize);
    weu_list_resize(h, h->count - 1, freeData);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  FROM TO
/////////////////////////////////////////////////////////////////////////////////////////////////////

void weu_list_insertCount(weu_list *h, uint32_t index, uint32_t count) {
    if (!h || !count)       return;
    if (index >= h->count)  index = h->count;
    weu_list_resize(h, h->count + count, false);
    memmove(h->data + ((index + count) * h->dataSize), h->data + (index * h->dataSize), (h->count - index - count) * h->dataSize);
    memset(h->data + (index * h->dataSize), 0, count * h->dataSize);
}
void weu_list_insertFromTo(weu_list *h, uint32_t from, uint32_t to) {
    if (!h) return;
    if (from > to) from = to;
    uint32_t count = to - from;
    weu_list_insertCount(h, from, count);
}

void weu_list_removeCount(weu_list *h, uint32_t index, uint32_t count, bool freeData) {
    if (!h || !count)       return;
    if (index >= h->count)  return;
    if (index + count > h->count) count = h->count - index;
    if (freeData && h->d != NULL) {
        for (uint32_t i = index; i < index + count; i++)
        {
            h->d(h->data + (i * h->dataSize));
        }
    }
    memmove(h->data + (index * h->dataSize), h->data + ((index + count) * h->dataSize), (h->count - index - count) * h->dataSize);
    weu_list_resize(h, h->count - count, false);
}
void weu_list_removeFromTo(weu_list *h, uint32_t from, uint32_t to, bool freeData) {
    if (!h || from > to) return;
    if (to > h->count) to = h->count;
    int32_t count = to - from;
    if (count < 0) return;
    weu_list_removeCount(h, from, count, freeData);
}

#endif
#endif