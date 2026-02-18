#ifndef WEU_UNROLLED_LINK_LIST_H
#define WEU_UNROLLED_LINK_LIST_H

#define WEUDEF extern

#include "weu_datatypes.h"

WEUDEF weu_URLListNode *weu_URLList_new(uint32_t rollCapacity, uint32_t rollCount, uint32_t dataSize, datafreefun d);

WEUDEF void weu_URLList_setData(weu_URLListNode *h, uint32_t dataIndex, void *data);
WEUDEF void weu_URLList_getData(weu_URLListNode *h, uint32_t dataIndex, void *out);

WEUDEF void weu_URLList_setDataCurrent(weu_URLListNode *h, uint32_t dataIndex, void *data);
WEUDEF void weu_URLList_getDataCurrent(weu_URLListNode *h, uint32_t dataIndex, void *out);

WEUDEF void weu_URLList_seekBeg(weu_URLListNode *h);
WEUDEF void weu_URLList_seekEnd(weu_URLListNode *h);
WEUDEF bool weu_URLList_seekRoll(weu_URLListNode *h, uint32_t rollIndex);

#define WEU_IMPLEMENTATION
#ifdef WEU_IMPLEMENTATION

#include <stdlib.h>
#include <memory.h>

weu_URLListNode *weu_URLList_new(uint32_t rollCapacity, uint32_t rollCount, uint32_t dataSize, datafreefun d) {
    weu_URLListNode *node = NULL;
    for (uint32_t i = 0; i < rollCount; i++)
    {
        weu_URLListNode *next = (weu_URLListNode*)calloc(1, sizeof(weu_URLListNode));
        next->count = 0;
        next->capacity = rollCapacity;
        next->dataSize = dataSize;
        next->d = d;
        next->data = calloc(rollCapacity, dataSize);
        if (node) {
            next->prev = node;
            node->next = next;
        }
        node = next;
    }
    weu_URLList_seekBeg(node);
    return node;
}

void weu_URLList_setData(weu_URLListNode *h, uint32_t dataIndex, void *data) {
    if (h == NULL) return;
    uint32_t rollIndex = dataIndex / h->capacity;
    if (weu_URLList_seekRoll(h, rollIndex)) {
        uint32_t index = dataIndex - (rollIndex * h->capacity);
        memmove(h->data + (index * h->dataSize), data, h->dataSize);
    }
}
void weu_URLList_getData(weu_URLListNode *h, uint32_t dataIndex, void *out) {
    if (h == NULL) return;
    uint32_t rollIndex = dataIndex / h->capacity;
    if (weu_URLList_seekRoll(h, rollIndex)) {
        uint32_t index = dataIndex - (rollIndex * h->capacity);
        printf("%i | %i\n", rollIndex, index);
        memmove(out, h->data + (index * h->dataSize), h->dataSize);
    }
}

void weu_URLList_setDataCurrent(weu_URLListNode *h, uint32_t dataIndex, void *data) {
    if (!h) return;
    if (dataIndex >= h->capacity) return;
    memmove(h->data + (dataIndex * h->dataSize), data, h->dataSize);
}
void weu_URLList_getDataCurrent(weu_URLListNode *h, uint32_t dataIndex, void *out) {
    if (!h) return;
    if (dataIndex >= h->capacity) return;
    memmove(out, h->data + (dataIndex * h->dataSize), h->dataSize);
}

void weu_URLList_seekBeg(weu_URLListNode *h) {
    if (!h) return;
    while (h->prev != NULL) { h = h->prev; }
}
void weu_URLList_seekEnd(weu_URLListNode *h) {
    if (!h) return;
    while (h->next != NULL) { h = h->next; }
}
bool weu_URLList_seekRoll(weu_URLListNode *h, uint32_t rollIndex) {
    if (!h) return false;
    weu_URLList_seekBeg(h);
    bool found = true;
    for (uint32_t i = 0; i <= rollIndex; i++)
    {
        if (h->next != NULL) { h = h->next; }
        else {
            if (i == rollIndex) return true;
            else                found = false;
        }
    }
    return found;
}

#endif
#endif