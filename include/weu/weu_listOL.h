#ifndef WEU_LIST_OF_LISTS_H
#define WEU_LIST_OF_LISTS_H

#define WEUDEF extern

#include "weu_datatypes.h"

#define WEU_LIST_INDEX_INVALID 0xffffffff
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  ALLOCATION

WEUDEF weu_listOL *weu_listOL_new(uint32_t listCount, uint32_t listCapacity, uint32_t dataSize, datafreefun d);
WEUDEF void weu_listOL_free(weu_listOL **h, bool freeData);
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  DATA

WEUDEF void weu_listOL_setData(weu_listOL *h, uint32_t index, void *data);
WEUDEF void weu_listOL_getData(weu_listOL *h, uint32_t index, void *out);
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  DATA AT LIST

WEUDEF void weu_listOL_setDataAt(weu_listOL *h, uint32_t listIndex, uint32_t dataIndex, void *data);
WEUDEF void weu_listOL_getDataAt(weu_listOL *h, uint32_t listIndex, uint32_t dataIndex, void *out);
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  CLEAR SINGLE

WEUDEF void weu_listOL_clearDataAtIndex(weu_listOL *h, uint32_t index, bool freeData);
WEUDEF void weu_listOL_clearDataAtList(weu_listOL *h, uint32_t listIndex, uint32_t dataIndex, bool freeData);
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  CLEAR LIST

WEUDEF void weu_listOL_clear(weu_listOL *h, bool freeData);
WEUDEF void weu_listOL_clearList(weu_listOL *h, uint32_t listIndex, bool freeData);

#ifdef WEU_IMPLEMENTATION

#include <stdlib.h>
#include <memory.h>

/////////////////////////////////////////////////////////////////////////////////////////////////////
//  ALLOCATION
/////////////////////////////////////////////////////////////////////////////////////////////////////

weu_listOL *weu_listOL_new(uint32_t listCount, uint32_t listCapacity, uint32_t dataSize, datafreefun d) {
    weu_listOL *out     = (weu_listOL*)calloc(1, sizeof(weu_listOL));
    out->listCount      = listCount;
    out->listCapacity   = listCapacity;
    out->dataSize       = dataSize;
    out->d              = d;
    out->data = (void**)calloc(listCount, sizeof(void*));
    for (uint32_t i = 0; i < listCount; i++)
    {
        out->data[i] = calloc(listCapacity, dataSize);
    }
    return out;
}
void weu_listOL_free(weu_listOL **h, bool freeData) {
    if (!*h) return;
    weu_listOL *list = *h;
    for (uint32_t i = 0; i < list->listCount; i++)
    {
        if (freeData && list->d) {
            for (uint32_t j = 0; j < list->listCapacity; j++)
            {
                list->d(list->data[i] + (j * list->dataSize));
            }
        }
        free(list->data[i]);
    }
    free(list->data);
    free(list);
    *h = NULL;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  DATA
/////////////////////////////////////////////////////////////////////////////////////////////////////

void weu_listOL_setData(weu_listOL *h, uint32_t index, void *data) {
    if (!h) return;
    uint32_t listIndex = index / h->listCapacity;
    if (listIndex < h->listCount) {
        uint32_t dataIndex = index - (listIndex * h->listCapacity);
        memmove(h->data[listIndex] + (dataIndex * h->dataSize), data, h->dataSize);
    }
}
void weu_listOL_getData(weu_listOL *h, uint32_t index, void *out) {
    if (!h) return;
    uint32_t listIndex = index / h->listCapacity;
    if (listIndex < h->listCount) {
        uint32_t dataIndex = index - (listIndex * h->listCapacity);
        memmove(out, h->data[listIndex] + (dataIndex * h->dataSize), h->dataSize);
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  DATA AT LIST
/////////////////////////////////////////////////////////////////////////////////////////////////////

void weu_listOL_setDataAt(weu_listOL *h, uint32_t listIndex, uint32_t dataIndex, void *data) {
    if (!h) return;
    if (listIndex >= h->listCount || dataIndex >= h->listCapacity) return;
    memmove(h->data[listIndex] + (dataIndex * h->dataSize), data, h->dataSize);
}
void weu_listOL_getDataAt(weu_listOL *h, uint32_t listIndex, uint32_t dataIndex, void *out) {
    if (!h) { out = NULL; return; }
    if (listIndex >= h->listCount || dataIndex >= h->listCapacity) { out = NULL; return; }
    memmove(out, h->data[listIndex] + (dataIndex * h->dataSize), h->dataSize);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  CLEAR SINGLE
/////////////////////////////////////////////////////////////////////////////////////////////////////

void weu_listOL_clearDataAtIndex(weu_listOL *h, uint32_t index, bool freeData) {
    if (!h) return;
    if (index >= h->listCount * h->listCapacity) return;
    uint32_t listIndex = index / h->listCapacity;
    if (listIndex < h->listCount) {
        uint32_t dataIndex = index - (listIndex * h->listCapacity);
        if (freeData && h->d) {
            h->d(h->data[listIndex] + (dataIndex * h->dataSize));
        }
        memset(h->data[listIndex] + (dataIndex * h->dataSize), 0, h->dataSize);
    }
}
void weu_listOL_clearDataAtList(weu_listOL *h, uint32_t listIndex, uint32_t dataIndex, bool freeData) {
    if (!h) return;
    if (listIndex >= h->listCount || dataIndex >= h->listCapacity) return;
    if (freeData && h->d) {
        h->d(h->data[listIndex] + (dataIndex * h->dataSize));
    }
    memset(h->data[listIndex] + (dataIndex * h->dataSize), 0, h->dataSize);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  CLEAR LIST
/////////////////////////////////////////////////////////////////////////////////////////////////////

void weu_listOL_clear(weu_listOL *h, bool freeData) {
    if (!h) return;
    for (uint32_t i = 0; i < h->listCount; i++)
    {
        if (freeData && h->d) {
            for (uint32_t j = 0; j < h->listCapacity; j++)
            {
                h->d(h->data[i] + (j * h->dataSize));
            }
        }
        memset(h->data[i], 0, h->listCapacity * h->dataSize);
    }
}
void weu_listOL_clearList(weu_listOL *h, uint32_t listIndex, bool freeData) {
    if (!h) return;
    if (listIndex >= h->listCount) return;
    if (freeData && h->d) {
        for (uint32_t i = 0; i < h->listCapacity; i++)
        {
            h->d(h->data[listIndex] + (i * h->dataSize));
        }
    }
    memset(h->data[listIndex], 0, h->listCapacity * h->dataSize);
}

#endif
#endif