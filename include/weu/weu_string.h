#ifndef weu_string_h
#define weu_string_h

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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/////////////////////////////////////////////////////////////////////////////////////////////////////
//  MACROS

//  stringify
#define STRY(X)     (#X)
//  token pasting
#define TOKP(X, Y)  (X#Y)
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  ALLOCATION

WEUDEF weu_string *weu_string_newSize(int length) {
    weu_string *out         = (weu_string*)malloc(sizeof(weu_string));
    out->length         = length;
    out->text           = (char*)calloc(length + 1, 1);
    out->text[length]   = '\0';
    return out;
}
WEUDEF weu_string *weu_string_new(const char *text) {
    int length  = strlen(text);
    weu_string *out = weu_string_newSize(length);
    memcpy(out->text, text, length);
    return out;
}
WEUDEF weu_string *weu_string_copy(weu_string *str) {
    if (str == NULL) return NULL;
    weu_string *out = weu_string_newSize(str->length);
    memcpy(out->text, str->text, str->length);
    return out;
}

WEUDEF void weu_string_resize(weu_string *s, int length, char emptyFill) {
    s->text = (char*)realloc(s->text, length + 1);
    if (s->length < length) {
        memset(s->text + s->length, emptyFill, length - s->length);
    }
    s->length = length;
    s->text[length] = '\0'; 
}

WEUDEF void weu_string_free(weu_string **data) {
    if (*data == NULL) return;
    free((*data)->text);
    free( *data);
    *data = NULL;
}
WEUDEF void weu_string_listFree(void **data) {
    if (*data == NULL) return;
    free(((weu_string*)*data)->text);
    free((weu_string*)*data);
    *data = NULL;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  LENGTH

WEUDEF int weu_string_stringLength(const weu_string *data) {
    if (data == NULL) return -1;
    return data->length;
}
WEUDEF int weu_string_textLength(const char *text) {
    return strlen(text);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  COMPARISON

WEUDEF int weu_string_matches(const weu_string *s1, const weu_string *s2) {
    if (s1 == NULL || s2 == NULL) return 0;
    if (s1->length != s2->length) return 0;
    return strcmp(s1->text, s2->text) == 0 ? 1 : 0;
}
WEUDEF int weu_string_textMatches(const char *text1, const char *text2) {
    return strcmp(text1, text2) == 0 ? 1 : 0;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  TEXT

WEUDEF void weu_string_setText(weu_string *s, const char *text) {
    if (s == NULL) return;
    int len = weu_string_textLength(text);
    s->length = len;
    s->text = (char*)realloc(s->text, len + 1);
    memcpy(s->text, text, len);
    s->text[len] = '\0';
}

WEUDEF weu_string *weu_string_fromTo(const weu_string *s, int from, int to) {
    if (s == NULL) return NULL;
    from = from > 0 ? from : 0;
    to = to > from ? (to < s->length ? to : s->length) : from;
    int len = to - from;
    weu_string *out = weu_string_newSize(len);
    memcpy(out->text, s->text + from, len);
    return out;
}
WEUDEF void weu_string_deleteFromTo(weu_string *s, int from, int to) {
    if (s == NULL) return;
    from = from > 0 ? from : 0;
    to = to > from ? (to < s->length ? to : s->length) : from;
    int len = to - from;
    for (int i = from + len; i < s->length; i++) {
        s->text[i - from - len] = s->text[i];  
    }
    weu_string_resize(s, s->length - len, ' ');
}
WEUDEF weu_string *weu_string_cutFromTo(weu_string *s, int from, int to) {
    if (s == NULL) return NULL;
    from = from > 0 ? from : 0;
    to = to > from ? (to < s->length ? to : s->length) : from;
    int len = to - from;
    weu_string *out = weu_string_newSize(len);
    memcpy(out->text, s->text + from, len);
    weu_string_deleteFromTo(s, from, to);
    return out;
}

WEUDEF void weu_string_overwriteFromTo(weu_string *s, int from, int to, const char *text) {
    if (s == NULL) return;
    from = from > 0 ? from : 0;
    to = to > from ? (to < s->length ? to : s->length) : from;
    int len = weu_string_textLength(text);
    len = to - from < len ? to - from : len;
    memcpy(s->text + from, text, len);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  APPEND

WEUDEF void weu_string_append(weu_string *s1, weu_string *s2) {
    if (s1 == NULL || s2 == NULL) return;
    weu_string_resize(s1, s1->length + s2->length, ' ');
    memcpy(s1->text + s1->length, s2->text, s2->length);
}
WEUDEF void weu_string_appendText(weu_string *s, const char *text) {
    if (s == NULL) return;
    int textLen = weu_string_textLength(text);
    weu_string_resize(s, s->length + textLen, ' ');
    memcpy(s->text + s->length, text, textLen);
}

WEUDEF weu_string *weu_string_getAppend(weu_string *s1, weu_string *s2) {
    if (s1 == NULL || s2 == NULL) return NULL;
    weu_string *out = weu_string_newSize(s1->length + s2->length);
    memcpy(out->text, s1->text, s1->length);
    memcpy(out->text + s1->length, s2->text, s2->length);
    return out;
}
WEUDEF weu_string *weu_string_getAppendText(weu_string *s, const char *text) {
    if (s == NULL) return NULL;
    int textLen = weu_string_textLength(text);
    weu_string *out = weu_string_newSize(s->length + textLen);
    memcpy(out->text, s->text, s->length);
    memcpy(out->text + s->length, text, textLen);
    return out;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  LINE

WEUDEF weu_string *weu_string_getLine(const weu_string *s) {
    if (s == NULL) return NULL;
    int charPointer = 0;
    while (charPointer < s->length) {
        if (s->text[charPointer] == '\n' || s->text[charPointer] == '\0') break;
        ++charPointer;
    }
    
    // for (charPointer; charPointer < s->length; charPointer++) {
    //     if (s->text[charPointer] == '\n' || s->text[charPointer] == '\0') {
    //         break;
    //     }
    // }
    weu_string *out = weu_string_fromTo(s, 0, charPointer);
    return out;
}
WEUDEF weu_string *weu_string_cutLine(weu_string *s) {
    if (s == NULL) return NULL;
    int isNewLine = 0;
    int charPointer = 0;
    while (charPointer < s->length) {
        if (s->text[charPointer] == '\n') {
            isNewLine = 1;
            break;
        }
        else if (s->text[charPointer] == '\0') break;
        ++charPointer;
    }
    
    // for (charPointer; charPointer < s->length; charPointer++) {
    //     if (s->text[charPointer] == '\n') {
    //         isNewLine = 1;
    //         break;
    //     }
    //     else if (s->text[charPointer] == '\0') {
    //         break;
    //     }
    // }
    weu_string *out = weu_string_fromTo(s, 0, charPointer);
    weu_string_deleteFromTo(s, 0, charPointer + isNewLine);
    return out;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  SPLIT

WEUDEF weu_list *weu_string_splitByChar(const weu_string *s, char c) {
    if (s == NULL) return NULL;
    weu_list *out = weu_list_new(0, weu_string_listFree, ALLOC_MALLOC);
    int sbeg = 0;
    for (int i = 0; i < s->length; i++) {
        if (s->text[i] == c) {
            weu_list_push(out, weu_string_fromTo(s, sbeg, i));
            sbeg = i + 1;
        }
    }
    if (sbeg < s->length) {
        weu_list_push(out, weu_string_fromTo(s, sbeg, s->length));
    }
    return out;
}
WEUDEF weu_list *weu_string_splitByText(const weu_string *s, const char *text) {
    if (s == NULL || text == NULL) return NULL;
    weu_list *out = weu_list_new(0, weu_string_listFree, ALLOC_MALLOC);
    int textLen = strlen(text);
    int sbeg = 0;
    for (int i = 0; i < s->length; i++) {
        int match = 0;
        for (int j = 0; j < textLen; j++) {
            if (s->text[i + j] == text[j]) ++match;
            else {
                i += j;
                break;
            }
        }
        if (match == textLen) {
            weu_list_push(out, weu_string_fromTo(s, sbeg, i));
            i += textLen;
            sbeg = i;
        }
        match = 0;
    }
    if (sbeg < s->length) weu_list_push(out, weu_string_fromTo(s, sbeg, s->length));
    return out;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  PARSE

WEUDEF int weu_string_parseInt(const char *text) {
    return atoi(text);
}
WEUDEF float weu_string_parseFloat(const char *text) {
    return atof(text);
}
WEUDEF long long weu_string_parseLLong(const char *text) {
    return atoll(text);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  TO weu_string

WEUDEF weu_string *weu_string_int(int val) {
    char buffer[64];
    sprintf(buffer, "%i", val);
    weu_string *out = weu_string_new(buffer);
    return out;
}
WEUDEF weu_string *weu_string_float(float val) {
    char buffer[64];
    sprintf(buffer, "%f", val);
    weu_string *out = weu_string_new(buffer);
    return out;
}
WEUDEF weu_string *weu_string_llong(long long val) {
    char buffer[64];
    sprintf(buffer, "%lld", val);
    weu_string *out = weu_string_new(buffer);
    return out;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  DEBUG

WEUDEF void weu_string_print(const char *text, const char *pre, const char *post) {
    printf("%s%s%s\n", pre, text, post);
}

#endif
#endif