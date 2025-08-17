#ifndef weu_hashtable_h
#define weu_hashtable_h

#ifndef WEUDEF
    #ifdef WEU_EXTERN
    #define WEUDEF extern
    #else
    #define WEUDEF static
    #define WEU_IMPLEMENTATION
    #endif
#endif

#ifdef WEU_IMPLEMENTATION

#include "weu_datatypes.h"
#include "weu_list.h"
#include "weu_string.h"

#include <stdlib.h>

#define MIN_TABLE_SIZE      16

#define FNV_PRIME_32        0x01000193
#define FNV_OFF_BASIS_32    0x811c9dc5

/////////////////////////////////////////////////////////////////////////////////////////////////////
//  HASH

WEUDEF unsigned int weu_hash_FNV(const char *str, unsigned int len) {
    unsigned int hash = FNV_OFF_BASIS_32;
    for (int i = 0; i < len; i++) {
        hash ^= str[i];
        hash *= FNV_PRIME_32;
    }
    return hash % len;
}
WEUDEF unsigned int weu_hash_strFNV(string *str) {
    return weu_hash_FNV(str->text, str->length);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  ALLOCATION

WEUDEF hashTable *weu_hashtable_new(unsigned int size, datafreefun d) {
    size = size > MIN_TABLE_SIZE ? size : MIN_TABLE_SIZE;
    hashTable *out = (hashTable*)malloc(sizeof(hashTable));
    out->length = size;
    out->data = (hashItem*)malloc(sizeof(hashItem) * size);
    if (d != NULL) out->d = d; else out->d = NULL;
    for (int i = 0; i < size; i++) {
        out->data[i].inUse = 0;
        out->data[i].value = NULL;
        out->data[i].key = weu_string_new("");
    }
    return out;
}
WEUDEF void weu_hashtable_free(hashTable **handle) {
    hashTable *table = (*handle);
    for (int i = 0; i < table->length; i++) {
        weu_string_free(&table->data[i].key);
        if (table->d != NULL) table->d(&table->data[i].value);
    }
    free((*handle)->data);
    free(*handle);
    *handle = NULL;
}

WEUDEF void weu_hashtable_addItem(hashTable *table, string *key, void *value, int freeKeyOnDone) {
    if (table == NULL || key == NULL) return;
    unsigned int hashValue = weu_hash_FNV(key->text, table->length);
    int position = hashValue;
    do
    {
        if (position >= table->length) position = 0;
        if (weu_string_matches(table->data[position].key, key)) {
            printf("Hashtable already contains key - %s\n", key->text);
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
WEUDEF void weu_hashtable_removeItem(hashTable *table, string *key, int freeKeyOnDone) {
    if (table == NULL || key == NULL) return;
    unsigned int hashValue = weu_hash_FNV(key->text, table->length);
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
    if (!containsKey) printf("Hashtable does not conatin key - %s\n", key->text);
    if (freeKeyOnDone) weu_string_free(&key);
}

WEUDEF void* weu_hashtable_getValue(hashTable *table, string *key, int freeKeyOnDone) {
    if (table == NULL || key == NULL) return NULL;
    unsigned int hashValue = weu_hash_FNV(key->text, table->length);
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
    if (!containsKey) printf("Hashtable does not contain key - %s\n", key->text);
    if (freeKeyOnDone) weu_string_free(&key);
    return out;
}
WEUDEF void* weu_hashtable_getValueByIndex(hashTable *table, int index) {
    if (index < 0 || index >= table->length) return NULL;
    return table->data[index].value;
}
WEUDEF string* weu_hashtable_getKeyByIndex(hashTable *table, int index) {
    if (index < 0 || index >= table->length) return NULL;
    return table->data[index].key;
}

WEUDEF int weu_hashtable_getKeyIndex(hashTable *table, string *key, int freeKeyOnDone) {
    if (table == NULL || key == NULL) return -1;
    unsigned int hashValue = weu_hash_FNV(key->text, table->length);
    int out         = -1;
    int position    = hashValue;
    do
    {
        if (position >= table->length) position = 0;
        if (table->data[position].inUse && weu_string_matches(table->data[position].key, key)) {
            out = position; break; 
        }
    } while (++position != hashValue);
    if (out == -1) printf("Hashtable does not contain key - %s\n", key->text);
    if (freeKeyOnDone) weu_string_free(&key);
    return out;
}

#endif
#endif