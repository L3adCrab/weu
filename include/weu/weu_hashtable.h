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

#ifndef weu_hashtable_h
#define weu_hashtable_h

#ifndef WEUDEF
    #ifdef WEU_STATIC
    #define WEUDEF static
    #define WEU_IMPLEMENTATION
    #else
    #define WEUDEF extern
    #endif
#endif

#include "weu_datatypes.h"
#include "weu_string.h"

#include <stdlib.h>

#define MIN_TABLE_SIZE      16

#define FNV_PRIME_32        0x01000193
#define FNV_OFF_BASIS_32    0x811c9dc5

/////////////////////////////////////////////////////////////////////////////////////////////////////
//  HASH

WEUDEF unsigned int weu_hash_FNV(const char *str, unsigned int len);
WEUDEF unsigned int weu_hash_strFNV(weu_string *str);
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  ALLOCATION

WEUDEF weu_hashTable *weu_hashtable_new(unsigned int size, datafreefun d);
WEUDEF void weu_hashtable_free(weu_hashTable **handle);
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  DATA

WEUDEF void* weu_hashtable_getValue(weu_hashTable *table, weu_string *key, bool freeKeyOnDone);
WEUDEF void* weu_hashtable_getValueByIndex(weu_hashTable *table, int index);

WEUDEF void weu_hashtable_addItem(weu_hashTable *table, weu_string *key, void *value, bool freeKeyOnDone);
WEUDEF void weu_hashtable_removeItem(weu_hashTable *table, weu_string *key, bool freeKeyOnDone);
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  INDEX

WEUDEF int weu_hashtable_getUsableIndex(weu_hashTable *table, weu_string *key, bool freeKeyOnDone);
WEUDEF void weu_hashtable_setDataAtIndex(weu_hashTable *table, int index, weu_string *key, void *data, bool freeKeyOnDone);
WEUDEF void weu_hashtable_removeItemAtIndex(weu_hashTable *table, int index);
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  GET KEY

WEUDEF weu_string* weu_hashtable_getKeyByIndex(weu_hashTable *table, int index);
WEUDEF int weu_hashtable_getKeyIndex(weu_hashTable *table, weu_string *key, bool freeKeyOnDone);

#ifdef WEU_IMPLEMENTATION
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  HASH
/////////////////////////////////////////////////////////////////////////////////////////////////////

unsigned int weu_hash_FNV(const char *str, unsigned int len) {
    uint32_t hash = FNV_OFF_BASIS_32;
    for (int i = 0; i < len; i++) {
        hash ^= str[i];
        hash *= FNV_PRIME_32;
    }
    return hash;
}
unsigned int weu_hash_strFNV(weu_string *str) {
    return weu_hash_FNV(str->text, str->length);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  ALLOCATION
/////////////////////////////////////////////////////////////////////////////////////////////////////

weu_hashTable *weu_hashtable_new(unsigned int size, datafreefun d) {
    size = size > MIN_TABLE_SIZE ? size : MIN_TABLE_SIZE;
    weu_hashTable *out = (weu_hashTable*)malloc(sizeof(weu_hashTable));
    out->length = size;
    out->data = (weu_hashItem*)malloc(sizeof(weu_hashItem) * size);
    if (d != NULL) out->d = d; else out->d = NULL;
    for (int i = 0; i < size; i++) {
        out->data[i].inUse = 0;
        out->data[i].value = NULL;
        out->data[i].key = weu_string_new("");
    }
    return out;
}
void weu_hashtable_free(weu_hashTable **handle) {
    weu_hashTable *table = (*handle);
    for (int i = 0; i < table->length; i++) {
        weu_string_free(&table->data[i].key);
        if (table->d != NULL) table->d(&table->data[i].value);
    }
    free((*handle)->data);
    free(*handle);
    *handle = NULL;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  DATA
/////////////////////////////////////////////////////////////////////////////////////////////////////

void* weu_hashtable_getValue(weu_hashTable *table, weu_string *key, bool freeKeyOnDone) {
    if (table == NULL || key == NULL) return NULL;
    unsigned int hashValue = weu_hash_strFNV(key) % table->length;
    int position    = hashValue;
    int containsKey = 0;
    void *out       = NULL;
    do
    {
        if (position >= table->length) position = 0;
        if (table->data[position].inUse && weu_string_matches(table->data[position].key, key)) {
            out = table->data[position].value;
            containsKey = 1;
            break;
        }
    } while (++position != hashValue);
    if (!containsKey) printf("weu_hashTable does not contain key - %s\n", key->text);
    if (freeKeyOnDone) weu_string_free(&key);
    return out;
}
void* weu_hashtable_getValueByIndex(weu_hashTable *table, int index) {
    if (index < 0 || index >= table->length) return NULL;
    return table->data[index].value;
}

void weu_hashtable_addItem(weu_hashTable *table, weu_string *key, void *value, bool freeKeyOnDone) {
    if (table == NULL || key == NULL) return;
    unsigned int hashValue = weu_hash_strFNV(key) % table->length;
    int position = hashValue;
    do
    {
        if (position >= table->length) position = 0;
        if (weu_string_matches(table->data[position].key, key)) {
            printf("weu_hashTable already contains key - %s\n", key->text);
            break;
        }
        if (!table->data[position].inUse) {
            table->data[position].inUse = 1;
            table->data[position].value = value;
            weu_string_setText(table->data[position].key, key->text);
            break;
        }
    } while (++position == hashValue);
    if (freeKeyOnDone) weu_string_free(&key);
}
void weu_hashtable_removeItem(weu_hashTable *table, weu_string *key, bool freeKeyOnDone) {
    if (table == NULL || key == NULL) return;
    unsigned int hashValue = weu_hash_strFNV(key) % table->length;
    int position    = hashValue;
    int containsKey = 0;
    do
    {
        if (position >= table->length) position = 0;
        if (table->data[position].inUse && weu_string_matches(table->data[position].key, key)) {
            table->data[position].inUse = 0;
            weu_string_setText(table->data[position].key, "");
            if (table->d != NULL) table->d(&table->data[position].value);
            table->data[position].value = NULL;
            containsKey = 1;
            break;
        }
    } while (++position != hashValue);
    if (!containsKey) printf("weu_hashTable does not conatin key - %s\n", key->text);
    if (freeKeyOnDone) weu_string_free(&key);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  INDEX
/////////////////////////////////////////////////////////////////////////////////////////////////////

int weu_hashtable_getUsableIndex(weu_hashTable *table, weu_string *key, bool freeKeyOnDone) {
    if (table == NULL || key == NULL) return -1;
    unsigned int hashValue = weu_hash_strFNV(key) % table->length;
    int position = hashValue;
    do
    {
        if (position >= table->length) position = 0;
        if (weu_string_matches(table->data[position].key, key)) {
            printf("weu_hashTable already contains key - %s\n", key->text);
            position = -1;
            break;
        }
        if (!table->data[position].inUse) {
            break;
        }
    } while (++position == hashValue);
    if (freeKeyOnDone) weu_string_free(&key);
    return position;
}
void weu_hashtable_setDataAtIndex(weu_hashTable *table, int index, weu_string *key, void *data, bool freeKeyOnDone) {
    if (index < 0 || index >= table->length) return;
    if (table->data[index].inUse) {
        weu_hashtable_removeItemAtIndex(table, index);    
    }
    table->data[index].inUse = 1;
    table->data[index].key = weu_string_copy(key);
    table->data[index].value = data;
    if (freeKeyOnDone) weu_string_free(&key);
}
void weu_hashtable_removeItemAtIndex(weu_hashTable *table, int index) {
    if (table == NULL || index < 0 || index >= table->length) return;
    table->data[index].inUse = false;
    if (table->d != NULL) table->d(&table->data[index].value);
    table->data[index].value = NULL;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  GET KEY
/////////////////////////////////////////////////////////////////////////////////////////////////////

weu_string* weu_hashtable_getKeyByIndex(weu_hashTable *table, int index) {
    if (index < 0 || index >= table->length) return NULL;
    return table->data[index].key;
}
int weu_hashtable_getKeyIndex(weu_hashTable *table, weu_string *key, bool freeKeyOnDone) {
    if (table == NULL || key == NULL) return -1;
    unsigned int hashValue = weu_hash_strFNV(key) % table->length;
    int out         = -1;
    int position    = hashValue;
    do
    {
        if (position >= table->length) position = 0;
        if (table->data[position].inUse && weu_string_matches(table->data[position].key, key)) {
            out = position; break; 
        }
    } while (++position != hashValue);
    if (out == -1) printf("weu_hashTable does not contain key - %s\n", key->text);
    if (freeKeyOnDone) weu_string_free(&key);
    return out;
}

#endif
#endif