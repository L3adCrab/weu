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

#ifndef weu_string_h
#define weu_string_h

#ifndef WEUDEF
    #ifdef WEU_STATIC
    #define WEUDEF static
    #define WEU_IMPLEMENTATION
    #else
    #define WEUDEF extern
    #endif
#endif

#include "weu_datatypes.h"
#include "weu_list.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

/////////////////////////////////////////////////////////////////////////////////////////////////////
//  MACROS

//  stringify
#define STRY(X)     (#X)
//  token pasting
#define TOKP(X, Y)  (X#Y)
#define SETLENRANGE(L, Min, Max) L = (int)L < Min ? Min : L > Max ? Max : L
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  ALLOCATION

WEUDEF weu_string *weu_string_newSize(int length);
WEUDEF weu_string *weu_string_new(const char *text);
WEUDEF weu_string *weu_string_copy(const weu_string *str);

WEUDEF void weu_string_resize(weu_string *s, int length, char emptyFill);

WEUDEF void weu_string_free(weu_string **data);
WEUDEF void weu_string_listFree(void **data);
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  NON ALLOC

WEUDEF weu_stringNA weu_stringNA_newSize(int length);
WEUDEF weu_stringNA weu_stringNA_newString(weu_string *str);
WEUDEF weu_stringNA weu_stringNA_newText(const char *text);
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  LENGTH

WEUDEF int weu_string_stringLength(const weu_string *data);
WEUDEF int weu_string_textLength(const char *text);
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  COMPARISON

WEUDEF int weu_string_matches(const weu_string *s1, const weu_string *s2);
WEUDEF int weu_string_textMatches(const char *text1, const char *text2);
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  TEXT

WEUDEF void weu_string_setText(weu_string *s, const char *text);
WEUDEF void weu_string_setTextNA(weu_stringNA *s, const char *text);

WEUDEF weu_string *weu_string_fromTo(const weu_string *s, int from, int to);
WEUDEF weu_stringNA weu_string_fromToNA(const weu_string *s, int from, int to);

WEUDEF weu_string *weu_string_cutFromTo(weu_string *s, int from, int to);
WEUDEF weu_stringNA weu_string_cutFromToNA(weu_string *s, int from, int to);

WEUDEF void weu_string_deleteFromTo(weu_string *s, int from, int to);
WEUDEF void weu_string_overwriteFromTo(weu_string *s, int from, int to, const char *text);
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  FILL

WEUDEF void weu_string_fill(weu_string *s, char fillChar, int from, int to);

WEUDEF weu_string *weu_string_filled(char fillChar, int len);
WEUDEF weu_stringNA weu_string_filledNA(char fillChar, int len);
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  APPEND

WEUDEF void weu_string_appendString(weu_string *s, int count, ...);
WEUDEF void weu_string_appendText(weu_string *s, int count, ...);

WEUDEF weu_stringNA weu_string_concatedStringNA(int count, ...);
WEUDEF weu_stringNA weu_string_concatedTextNA(int count, ...);
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  LINE

WEUDEF weu_string *weu_string_getLine(weu_string *s);
WEUDEF weu_stringNA weu_string_getLineNA(weu_string *s);

WEUDEF weu_string *weu_string_cutLine(weu_string *s);
WEUDEF weu_stringNA weu_string_cutLineNA(weu_string *s);
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  SPLIT

WEUDEF weu_list *weu_string_splitByChar(const weu_string *s, char c);
WEUDEF weu_list *weu_string_splitByText(const weu_string *s, const char *text);
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  PARSE

WEUDEF int weu_string_parseInt(const char *text);
WEUDEF float weu_string_parseFloat(const char *text);
WEUDEF long long weu_string_parseLLong(const char *text);
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  TO weu_string

WEUDEF weu_string *weu_string_float(float val);
WEUDEF weu_string *weu_string_int(int32_t val);
WEUDEF weu_string *weu_string_uint(uint32_t val);
WEUDEF weu_string *weu_string_llong(int64_t val);
WEUDEF weu_string *weu_string_ullong(uint64_t val);

WEUDEF weu_stringNA weu_string_floatNA(float val);
WEUDEF weu_stringNA weu_string_intNA(int32_t val);
WEUDEF weu_stringNA weu_string_uintNA(uint32_t val);
WEUDEF weu_stringNA weu_string_llongNA(int64_t val);
WEUDEF weu_stringNA weu_string_ullongNA(uint64_t val);
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  DEBUG

WEUDEF void weu_string_print(int count, ...);

#ifdef WEU_IMPLEMENTATION

/////////////////////////////////////////////////////////////////////////////////////////////////////
//  ALLOCATION
/////////////////////////////////////////////////////////////////////////////////////////////////////

weu_string *weu_string_newSize(int length) {
    weu_string *out     = (weu_string*)malloc(sizeof(weu_string));
    out->length         = length;
    out->text           = (char*)calloc(length + 1, 1);
    out->text[length]   = '\0';
    return out;
}
weu_string *weu_string_new(const char *text) {
    int length  = strlen(text);
    weu_string *out = weu_string_newSize(length);
    memcpy(out->text, text, length);
    return out;
}
weu_string *weu_string_copy(const weu_string *str) {
    if (str == NULL) return NULL;
    weu_string *out = weu_string_newSize(str->length);
    memcpy(out->text, str->text, str->length);
    return out;
}

void weu_string_resize(weu_string *s, int length, char emptyFill) {
    s->text = (char*)realloc(s->text, length + 1);
    if (s->length < length) {
        memset(s->text + s->length, emptyFill, length - s->length);
    }
    s->length = length;
    s->text[length] = '\0'; 
}

void weu_string_free(weu_string **data) {
    if (*data == NULL) return;
    free((*data)->text);
    free( *data);
    *data = NULL;
}
void weu_string_listFree(void **data) {
    if (*data == NULL) return;
    free(((weu_string*)*data)->text);
    free((weu_string*)*data);
    *data = NULL;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  NON ALLOC
/////////////////////////////////////////////////////////////////////////////////////////////////////

weu_stringNA weu_stringNA_newSize(int length) {
    weu_stringNA out;
    SETLENRANGE(length, 0, 511);
    out.length          = length;
    out.text[length]    = '\0';
    return out;
}
weu_stringNA weu_stringNA_newString(weu_string *str) {
    weu_stringNA out;
    out.length = str->length;
    SETLENRANGE(out.length, 0, 511);
    memcpy(out.text, str->text, out.length);
    out.text[out.length] = '\0';
    return out;
}
weu_stringNA weu_stringNA_newText(const char *text) {
    weu_stringNA out;
    uint32_t textLen = weu_string_textLength(text);
    SETLENRANGE(textLen, 0, 511);
    memcpy(out.text, text, textLen);
    out.text[textLen] = '\0';
    return out;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  LENGTH
/////////////////////////////////////////////////////////////////////////////////////////////////////

int weu_string_stringLength(const weu_string *data) {
    if (data == NULL) return -1;
    return data->length;
}
int weu_string_textLength(const char *text) {
    return strlen(text);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  COMPARISON
/////////////////////////////////////////////////////////////////////////////////////////////////////

int weu_string_matches(const weu_string *s1, const weu_string *s2) {
    if (s1 == NULL || s2 == NULL) return 0;
    if (s1->length != s2->length) return 0;
    return strcmp(s1->text, s2->text) == 0 ? 1 : 0;
}
int weu_string_textMatches(const char *text1, const char *text2) {
    return strcmp(text1, text2) == 0 ? 1 : 0;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  TEXT
/////////////////////////////////////////////////////////////////////////////////////////////////////

void weu_string_setText(weu_string *s, const char *text) {
    if (s == NULL) return;
    int len = weu_string_textLength(text);
    s->length = len;
    s->text = (char*)realloc(s->text, len + 1);
    memcpy(s->text, text, len);
    s->text[len] = '\0';
}
void weu_string_setTextNA(weu_stringNA *s, const char *text) {
    if (s == NULL) return;
    int len = weu_string_textLength(text);
    SETLENRANGE(len, 0, 511);
    s->length = len;
    memcpy(s->text, text, len);
    s->text[len] = '\0';
}

weu_string *weu_string_fromTo(const weu_string *s, int from, int to) {
    if (s == NULL) return NULL;
    from = from > 0 ? from : 0;
    to = to > from ? (to < s->length ? to : s->length) : from;
    int len = to - from;
    weu_string *out = weu_string_newSize(len);
    memcpy(out->text, s->text + from, len);
    return out;
}
weu_stringNA weu_string_fromToNA(const weu_string *s, int from, int to) {
    if (s == NULL) return (weu_stringNA){.length = 0, .text = ""};
    from = from > 0 ? from : 0;
    to = to > from ? (to < s->length ? to : s->length) : from;
    int len = to - from;
    SETLENRANGE(len, 0, 511);
    weu_stringNA out;
    out.length = len;
    memcpy(out.text, s->text + from, len);
    out.text[out.length] = '\0';
    return out;
}

weu_string *weu_string_cutFromTo(weu_string *s, int from, int to) {
    if (s == NULL) return NULL;
    from = from > 0 ? from : 0;
    to = to > from ? (to < s->length ? to : s->length) : from;
    int len = to - from;
    weu_string *out = weu_string_newSize(len);
    memcpy(out->text, s->text + from, len);
    weu_string_deleteFromTo(s, from, to);
    return out;
}
weu_stringNA weu_string_cutFromToNA(weu_string *s, int from, int to) {
    if (s == NULL) return (weu_stringNA){.length = 0, .text = ""};
    from = from > 0 ? from : 0;
    to = to > from ? (to < s->length ? to : s->length) : from;
    int len = to - from;
    SETLENRANGE(len, 0, 511);
    weu_stringNA out = weu_stringNA_newSize(len);
    memcpy(out.text, s->text + from, len);
    weu_string_deleteFromTo(s, from, to);
    return out;
}

void weu_string_deleteFromTo(weu_string *s, int from, int to) {
    if (s == NULL) return;
    from = from > 0 ? from : 0;
    to = to > from ? (to < s->length ? to : s->length) : from;
    int len = to - from;
    for (int i = from + len; i < s->length; i++) {
        s->text[i - from - len] = s->text[i];  
    }
    weu_string_resize(s, s->length - len, ' ');
}
void weu_string_overwriteFromTo(weu_string *s, int from, int to, const char *text) {
    if (s == NULL) return;
    from = from > 0 ? from : 0;
    to = to > from ? (to < s->length ? to : s->length) : from;
    int len = weu_string_textLength(text);
    len = to - from < len ? to - from : len;
    memcpy(s->text + from, text, len);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  FILL
/////////////////////////////////////////////////////////////////////////////////////////////////////

void weu_string_fill(weu_string *s, char fillChar, int from, int to) {
    if (s == NULL) return;
    to = to < 0 ? 0 : to;
    from = from < 0 ? 0 : from > to ? to : from;
    if (to > s->length) {
        weu_string_resize(s, to, ' ');
    }
    memset(&s->text[from], fillChar, to - from);
}

weu_string *weu_string_filled(char fillChar, int len) {
    weu_string *out = malloc(sizeof(weu_string));
    out->length = len + 1;
    out->text = (char*)malloc(len);
    memset(out->text, fillChar, len);
    out->text[out->length] = '\0';
    out->charPtrPos = 0;
    return out;
}
weu_stringNA weu_string_filledNA(char fillChar, int len) {
    weu_stringNA out;
    SETLENRANGE(len, 0, 511);
    out.length = len;
    memset(out.text, fillChar, len);
    out.text[len] = '\0';
    return out;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  APPEND
/////////////////////////////////////////////////////////////////////////////////////////////////////

void weu_string_appendString(weu_string *s, int count, ...) {
    if (s == NULL) return;
    va_list args;
    va_start(args, count);
    for (int i = 0; i < count; i++)
    {
        uint32_t oldLen = s->length;
        weu_string *str = va_arg(args, weu_string*);
        if (str == NULL) continue;
        weu_string_resize(s, s->length + str->length, ' ');
        memcpy(s->text + oldLen, str->text, str->length);
    }
    va_end(args);
}
void weu_string_appendText(weu_string *s, int count, ...) {
    if (s == NULL) return;
    va_list args;
    va_start(args, count);
    for (int i = 0; i < count; i++)
    {
        char *text = va_arg(args, char *);
        if (text == NULL) continue;
        int textLen = weu_string_textLength(text);
        int oldLen = s->length;
        weu_string_resize(s, s->length + textLen, ' ');
        memcpy(s->text + oldLen, text, textLen);
    }
    va_end(args);
}

weu_stringNA weu_string_concatedStringNA(int count, ...) {
    weu_stringNA out = weu_stringNA_newText("");
    va_list args;
    va_start(args, count);
    for (int i = 0; i < count; i++)
    {
        if (out.length >= 511) {
            break;
        }
        weu_string *str = va_arg(args, weu_string*);
        if (str == NULL) continue;
        int oldLen = out.length;
        int strLen = str->length;
        strLen = oldLen + strLen > 511 ? 511 - oldLen : strLen;
        out.length = oldLen + strLen;
        memcpy(out.text + oldLen, str->text, strLen);
    }
    va_end(args);
    out.text[out.length] = '\0';
    return out;
}
weu_stringNA weu_string_concatedTextNA(int count, ...) {
    weu_stringNA out = weu_stringNA_newText("");
    va_list args;
    va_start(args, count);
    for (int i = 0; i < count; i++)
    {
        if (out.length >= 511) {
            break;
        }
        char *str = va_arg(args, char*);
        if (str == NULL) continue;
        int oldLen = out.length;
        int strLen = weu_string_textLength(str);
        strLen = oldLen + strLen > 511 ? 511 - oldLen : strLen;
        out.length = oldLen + strLen;
        memcpy(out.text + oldLen, str, strLen);
    }
    va_end(args);
    out.text[out.length] = '\0';
    return out;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  LINE
/////////////////////////////////////////////////////////////////////////////////////////////////////

weu_string *weu_string_getLine(weu_string *s) {
    if (s == NULL) return NULL;
    int charPointer = s->charPtrPos;
    while (charPointer < s->length) {
        if (s->text[charPointer] == '\n' || s->text[charPointer] == '\0') break;
        ++charPointer;
    }
    weu_string *out = weu_string_fromTo(s, s->charPtrPos, charPointer);
    s->charPtrPos = ++charPointer;
    return out;
}
weu_stringNA weu_string_getLineNA(weu_string *s) {
    if (s == NULL) return (weu_stringNA){.length = 0, .text = ""};
    int charPointer = s->charPtrPos;
    while (charPointer < s->length) {
        if (s->text[charPointer] == '\n' || s->text[charPointer] == '\0') break;
        ++charPointer;
    }
    weu_stringNA out = weu_string_fromToNA(s, s->charPtrPos, charPointer);
    s->charPtrPos = ++charPointer;
    return out;
}

weu_string *weu_string_cutLine(weu_string *s) {
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
    weu_string *out = weu_string_fromTo(s, 0, charPointer);
    weu_string_deleteFromTo(s, 0, charPointer + isNewLine);
    return out;
}
weu_stringNA weu_string_cutLineNA(weu_string *s) {
    if (s == NULL) return (weu_stringNA){.length = 0, .text = ""};
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
    weu_stringNA out = weu_string_fromToNA(s, 0, charPointer);
    weu_string_deleteFromTo(s, 0, charPointer + isNewLine);
    return out;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  SPLIT
/////////////////////////////////////////////////////////////////////////////////////////////////////

weu_list *weu_string_splitByChar(const weu_string *s, char c) {
    if (s == NULL) return NULL;
    weu_list *out = weu_list_new(0, weu_string_listFree);
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
weu_list *weu_string_splitByText(const weu_string *s, const char *text) {
    if (s == NULL || text == NULL) return NULL;
    weu_list *out = weu_list_new(0, weu_string_listFree);
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
/////////////////////////////////////////////////////////////////////////////////////////////////////

int weu_string_parseInt(const char *text) {
    return atoi(text);
}
float weu_string_parseFloat(const char *text) {
    return atof(text);
}
long long weu_string_parseLLong(const char *text) {
    return atoll(text);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  TO STRING
/////////////////////////////////////////////////////////////////////////////////////////////////////

weu_string *weu_string_float(float val) {
    char buffer[64];
    sprintf(buffer, "%f", val);
    weu_string *out = weu_string_new(buffer);
    return out;
}
weu_string *weu_string_int(int32_t val) {
    char buffer[64];
    sprintf(buffer, "%i", val);
    weu_string *out = weu_string_new(buffer);
    return out;
}
weu_string *weu_string_uint(uint32_t val) {
    char buffer[64];
    sprintf(buffer, "%u", val);
    weu_string *out = weu_string_new(buffer);
    return out;
}
weu_string *weu_string_llong(int64_t val) {
    char buffer[64];
    sprintf(buffer, "%ld", val);
    weu_string *out = weu_string_new(buffer);
    return out;
}
weu_string *weu_string_ullong(uint64_t val) {
    char buffer[64];
    sprintf(buffer, "%lu", val);
    weu_string *out = weu_string_new(buffer);
    return out;
}

weu_stringNA weu_string_floatNA(float val) {
    char buffer[64];
    sprintf(buffer, "%f", val);
    return weu_stringNA_newText(buffer);
}
weu_stringNA weu_string_intNA(int32_t val) {
    char buffer[64];
    sprintf(buffer, "%i", val);
    return weu_stringNA_newText(buffer);
}
weu_stringNA weu_string_uintNA(uint32_t val) {
    char buffer[64];
    sprintf(buffer, "%u", val);
    return weu_stringNA_newText(buffer);
}
weu_stringNA weu_string_llongNA(int64_t val) {
    char buffer[64];
    sprintf(buffer, "%ld", val);
    return weu_stringNA_newText(buffer);
}
weu_stringNA weu_string_ullongNA(uint64_t val) {
    char buffer[64];
    sprintf(buffer, "%lu", val);
    return weu_stringNA_newText(buffer);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  DEBUG
/////////////////////////////////////////////////////////////////////////////////////////////////////

void weu_string_print(int count, ...) {
    va_list args;
    va_start(args, count);
    for (int i = 0; i < count; i++)
    {
        char *text = va_arg(args, char*);
        if (text == NULL) return;
        printf("%s", text);
    }
    va_end(args);
}

#endif
#endif