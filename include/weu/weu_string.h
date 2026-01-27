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
#include "weu_bitfield.h"

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
#define SETLENRANGE(L, Min, Max) L = (uint32_t)L <= Min ? Min : L > Max ? Max : L
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  ALLOCATION

WEUDEF weu_string *weu_string_newSize(uint32_t length);
WEUDEF weu_string *weu_string_new(const char *text);
WEUDEF weu_string *weu_string_newChar(char c);
WEUDEF weu_string *weu_string_copy(const weu_string *str);

//  Return copy of string with same memory of text.
//  Should not be edited or passed in funtions that edit string(non const).
//  Printf will still print until original strings terminator.
WEUDEF weu_string weu_string_slice(const weu_string *s, uint32_t from, uint32_t to);

WEUDEF void weu_string_resize(weu_string *s, uint32_t length, char emptyFill);

WEUDEF void weu_string_free(weu_string **data);
WEUDEF void weu_string_listFree(void **data);
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  NON ALLOC

WEUDEF weu_stringNA weu_stringNA_newSize(uint32_t length);
WEUDEF weu_stringNA weu_stringNA_new(const char *text);
WEUDEF weu_stringNA weu_stringNA_newChar(char c);
WEUDEF weu_stringNA weu_stringNA_newString(const weu_string *str);
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  LENGTH

WEUDEF uint32_t weu_string_stringLength(const weu_string *data);
WEUDEF uint32_t weu_string_textLength(const char *text);
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  COMPARISON

WEUDEF bool weu_string_matches(const weu_string *s1, const weu_string *s2);
WEUDEF bool weu_stringNA_matches(const weu_stringNA s1, const weu_stringNA s2);
WEUDEF bool weu_string_textMatches(const char *text1, const char *text2);
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  STRING

WEUDEF void weu_string_setText(weu_string *s, const char *text);
WEUDEF void weu_string_setTextNA(weu_stringNA *s, const char *text);

WEUDEF weu_string *weu_string_fromTo(const weu_string *s, uint32_t from, uint32_t to);
WEUDEF weu_string *weu_string_cutFromTo(weu_string *s, uint32_t from, uint32_t to);

WEUDEF weu_stringNA weu_string_fromToNA(const weu_string *s, uint32_t from, uint32_t to);
WEUDEF weu_stringNA weu_string_cutFromToNA(weu_string *s, uint32_t from, uint32_t to);

WEUDEF weu_string *weu_string_textFromTo(const char *text, uint32_t from, uint32_t to);
WEUDEF weu_stringNA weu_stringNA_textFromTo(const char *text, uint32_t from, uint32_t to);

WEUDEF void weu_string_removeFromTo(weu_string *s, uint32_t from, uint32_t to);
WEUDEF void weu_string_overwriteFromTo(weu_string *s, uint32_t from, uint32_t to, const char *text);

WEUDEF bool weu_string_containsText(const weu_string *s, const char *text);
WEUDEF bool weu_stringNA_containsText(const weu_stringNA *s, const char *text);
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  STRING CHAR POINTER

WEUDEF uint32_t weu_string_getPointerPos(const weu_string *s);
WEUDEF void weu_string_setPointerPos(weu_string *s, uint32_t pos);
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  FILL

WEUDEF void weu_string_fill(weu_string *s, char fillChar, uint32_t from, uint32_t to);
WEUDEF weu_stringNA weu_stringNA_fill(weu_stringNA s, char fillChar, uint32_t from, uint32_t to);

WEUDEF weu_string *weu_string_filled(char fillChar, uint32_t len);
WEUDEF weu_stringNA weu_stringNA_filled(char fillChar, uint32_t len);
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  CONCATE

WEUDEF void weu_string_concateString(weu_string *s, uint8_t count, ...);
WEUDEF void weu_string_concateStringNA(weu_string *s, uint8_t count, ...);
WEUDEF void weu_string_concateText(weu_string *s, uint8_t count, ...);

WEUDEF weu_stringNA weu_stringNA_concatedString(uint8_t count, ...);
WEUDEF weu_stringNA weu_stringNA_concatedStringNA(uint8_t count, ...);
WEUDEF weu_stringNA weu_stringNA_concatedText(uint8_t count, ...);
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  LINE

WEUDEF weu_string *weu_string_getLine(const weu_string *s);
WEUDEF weu_stringNA weu_string_getLineNA(const weu_string *s);

WEUDEF weu_string *weu_string_cutLine(weu_string *s);
WEUDEF weu_stringNA weu_string_cutLineNA(weu_string *s);
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  SPLIT

WEUDEF weu_list *weu_string_splitByChar(const weu_string *s, char c);
WEUDEF weu_list *weu_string_splitByText(const weu_string *s, const char *text);
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  CHAR REMOVAL

WEUDEF void weu_string_removeChars(weu_string *s, char charToRemove);
WEUDEF void weu_string_removeCharsFromBeg(weu_string *s, char charToRemove);
WEUDEF void weu_string_removeCharsFromEnd(weu_string *s, char charToRemove);

WEUDEF weu_string *weu_string_removedChars(const weu_string *s, char charToRemove);
WEUDEF weu_string *weu_string_removedCharsFromBeg(const weu_string *s, char charToRemove);
WEUDEF weu_string *weu_string_removedCharsFromEnd(const weu_string *s, char charToRemove);

WEUDEF weu_stringNA weu_stringNA_removeChars(weu_stringNA s, char charToRemove);
WEUDEF weu_stringNA weu_stringNA_removeCharsFromBeg(weu_stringNA s, char charToRemove);
WEUDEF weu_stringNA weu_stringNA_removeCharsFromEnd(weu_stringNA s, char charToRemove);
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  INDENTATION

WEUDEF void weu_string_addIndent(weu_string *s, uint8_t count, uint8_t spaceCount);
WEUDEF weu_string *weu_string_addedIndent(const weu_string *s, uint8_t count, uint8_t spaceCount);

WEUDEF void weu_string_removeIndent(weu_string *s);
WEUDEF weu_string *weu_string_removedIndent(const weu_string *s);
WEUDEF weu_stringNA weu_string_removedIndentNA(const weu_string *s);
WEUDEF weu_stringNA weu_stringNA_removeIndent(const weu_stringNA s);
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  EXPRESSION

/* varyingStringOut - if passed in created list, pushes variable strings to it
isMatching - if passed reference to bitfield_32 sets successful varying string test bits to 1

#expression features
%c - test single character
%s - test string, reads testable string until \0 terminator char
Returns as fail if string length is 

#repeat tests
To test multiple characters or strings with same conditions 
can specify single digit from 0 to 9 after percent sign and before identifier.
0 and 1 set count to 1.
#example - %2c or %7s

#string end char
For testing string, string end character can be specified
as single character in brackets.
Up to five characters can be specified, rest will be ignored.
#example - %s{;}[]

test conditions should be in sqare brackets after char of string identifiers
example - %c[condition]

#conditions
is char in range - [a-zA-Z]
if char out of range - [!a-z]
if char is any of - [abcd]
if char is not any of - [!abcd]

can be mixed - [a-z!A-Z0123!567]
test0string - true
Test7String - false

#example success
test string - "test string A1 - 1234 0987 AbF9d A"
expression  - "test string %c[A-Z]%c[0-9] - %4s{ }[a-zA-Z0-9!a]"
varying out - A, 1, 1234, 0987, abF9d, a
isMatching  - 1, 1, 1,    1,    1,     1

#example fail
test string - "test string Ab - 1234 0987 abF9d a"
expression  - "test string %c[A-Z]%c[0-9] - %4s{ }[a-zA-Z0-9!a]"
varying out - A, b, 1234, 0987, abF9d, a
isMatching  - 1, 0, 1,    1,    0,     0
*/
WEUDEF bool weu_string_textMatchesExpression(const char *text, const char *expression, weu_list *varyingStringOut, weu_bitfield_32 *isMatching);
WEUDEF bool weu_string_charMatchesCondition(const uint8_t c, const char *condition);
WEUDEF bool weu_string_textMatchesCondition(const char *text, const char *condition);
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

WEUDEF void weu_string_printText(int count, ...);

#ifdef WEU_IMPLEMENTATION

#define INVALID 0xffffffff

/////////////////////////////////////////////////////////////////////////////////////////////////////
//  ALLOCATION
/////////////////////////////////////////////////////////////////////////////////////////////////////

weu_string *weu_string_newSize(uint32_t length) {
    weu_string *out     = (weu_string*)malloc(sizeof(weu_string));
    out->length         = length;
    out->allocLength    = length;
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
weu_string *weu_string_newChar(char c) {
    weu_string *out = weu_string_newSize(1);
    out->text[0] = c;
    return out;
}
weu_string *weu_string_copy(const weu_string *str) {
    if (str == NULL) return NULL;
    weu_string *out = weu_string_newSize(str->length);
    memcpy(out->text, str->text, str->length);
    return out;
}

weu_string weu_string_slice(const weu_string *s, uint32_t from, uint32_t to) {
    if (s == NULL) return (weu_string){0};
    if (from > to) {uint32_t temp; SWAPVAR(from, to, temp); }
    if (to > s->length) to = s->length;
    return (weu_string){.allocLength = 0, .charPtrPos = from, .length = to - from, .text = s->text + from};
}

void weu_string_resize(weu_string *s, uint32_t length, char emptyFill) {
    s->text = (char*)realloc(s->text, length + 1);
    if (s->length < length) {
        memset(s->text + s->length, emptyFill, length - s->length);
    }
    s->length = length;
    s->allocLength = length;
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

weu_stringNA weu_stringNA_newSize(uint32_t length) {
    weu_stringNA out;
    SETLENRANGE(length, 0, 511);
    out.length          = length;
    out.text[length]    = '\0';
    return out;
}
weu_stringNA weu_stringNA_new(const char *text) {
    weu_stringNA out;
    uint32_t textLen = weu_string_textLength(text);
    SETLENRANGE(textLen, 0, 511);
    memcpy(out.text, text, textLen);
    out.text[textLen] = '\0';
    out.length = textLen;
    return out;
}
weu_stringNA weu_stringNA_newChar(char c) {
    weu_stringNA out;
    out.length = 1;
    out.text[0] = c;
    out.text[1] = '\0';
    return out;
}
weu_stringNA weu_stringNA_newString(const weu_string *str) {
    weu_stringNA out;
    out.length = str->length;
    SETLENRANGE(out.length, 0, 511);
    memcpy(out.text, str->text, out.length);
    out.text[out.length] = '\0';
    return out;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  LENGTH
/////////////////////////////////////////////////////////////////////////////////////////////////////

uint32_t weu_string_stringLength(const weu_string *data) {
    if (data == NULL) return -1;
    return data->length;
}
uint32_t weu_string_textLength(const char *text) {
    return strlen(text);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  COMPARISON
/////////////////////////////////////////////////////////////////////////////////////////////////////

bool weu_string_matches(const weu_string *s1, const weu_string *s2) {
    if (s1 == NULL || s2 == NULL) return 0;
    if (s1->length != s2->length) return 0;
    return strcmp(s1->text, s2->text) == 0 ? 1 : 0;
}
bool weu_stringNA_matches(const weu_stringNA s1, const weu_stringNA s2) {
    if (s1.length != s2.length) return 0;
     return strcmp(s1.text, s2.text) == 0 ? 1 : 0;
}
bool weu_string_textMatches(const char *text1, const char *text2) {
    return strcmp(text1, text2) == 0 ? 1 : 0;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  STRING
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

weu_string *weu_string_fromTo(const weu_string *s, uint32_t from, uint32_t to) {
    if (s == NULL) return NULL;
    from = from > 0 ? from : 0;
    to = to > from ? (to < s->length ? to : s->length) : from;
    int len = to - from;
    weu_string *out = weu_string_newSize(len);
    memcpy(out->text, s->text + from, len);
    return out;
}
weu_string *weu_string_cutFromTo(weu_string *s, uint32_t from, uint32_t to) {
    if (s == NULL) return NULL;
    from = from > 0 ? from : 0;
    to = to > from ? (to < s->length ? to : s->length) : from;
    int len = to - from;
    weu_string *out = weu_string_newSize(len);
    memcpy(out->text, s->text + from, len);
    weu_string_removeFromTo(s, from, to);
    return out;
}

weu_stringNA weu_string_fromToNA(const weu_string *s, uint32_t from, uint32_t to) {
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
weu_stringNA weu_string_cutFromToNA(weu_string *s, uint32_t from, uint32_t to) {
    if (s == NULL) return (weu_stringNA){.length = 0, .text = ""};
    from = from > 0 ? from : 0;
    to = to > from ? (to < s->length ? to : s->length) : from;
    int len = to - from;
    SETLENRANGE(len, 0, 511);
    weu_stringNA out = weu_stringNA_newSize(len);
    memcpy(out.text, s->text + from, len);
    weu_string_removeFromTo(s, from, to);
    return out;
}

weu_string *weu_string_textFromTo(const char *text, uint32_t from, uint32_t to) {
    if (text == NULL) return NULL;
    if (from > to) { uint32_t temp; SWAPVAR(from, to, temp); }
    uint32_t textLen = strlen(text);
    to = to > textLen ? textLen : to;
    weu_string *out = weu_string_newSize(to - from);
    memcpy(out->text, text + from, out->length);
    return out;
}
weu_stringNA weu_stringNA_textFromTo(const char *text, uint32_t from, uint32_t to) {
    if (text == NULL) return (weu_stringNA){0};
    if (from > to) { uint32_t temp; SWAPVAR(from, to, temp); }
    uint32_t textLen = strlen(text);
    to = to > textLen ? textLen : to;
    weu_stringNA out = {0};
    out.length = to - from + 1;
    memcpy(&out.text[0], text + from, to - from);
    out.text[out.length] = '\0';
    return out; 
}

void weu_string_removeFromTo(weu_string *s, uint32_t from, uint32_t to) {
    if (s == NULL) return;
    from = from > 0 ? from : 0;
    to = to > from ? (to < s->length ? to : s->length) : from;
    memmove(&s->text[from], &s->text[to], s->length - to);
    s->length = s->length - to - from;
    s->text[s->length] = '\0';
}
void weu_string_overwriteFromTo(weu_string *s, uint32_t from, uint32_t to, const char *text) {
    if (s == NULL) return;
    from = from > 0 ? from : 0;
    to = to > from ? (to < s->length ? to : s->length) : from;
    uint32_t len = weu_string_textLength(text);
    len = to - from < len ? to - from : len;
    memcpy(s->text + from, text, len);
}

bool weu_string_containsText(const weu_string *s, const char *text) {
    if (s == NULL || text == NULL) return false;
    bool contains = false;
    uint32_t textLen = weu_string_textLength(text);
    uint32_t pos = s->charPtrPos;
    for (; pos < s->length; pos++)
    {
        if ( s->text[pos] == text[0] ) {
            uint32_t i = 1;
            for (; i < textLen; i++)
            {
                if (s->text[pos + i] != text[i]) {
                    pos += i;
                    i = INVALID;
                    break;
                }
            }
            if (i != INVALID) { 
                contains = true;
                break;
            };
        }
    }
    ((weu_string*)s)->charPtrPos = pos;
    return contains;
}
bool weu_stringNA_containsText(const weu_stringNA *s, const char *text) {
    if (s == NULL || text == NULL) return false;
    bool contains = false;
    uint32_t textLen = weu_string_textLength(text);
    for (uint32_t i = 0; i < s->length; i++)
    {
        if ( s->text[i] == text[0] ) {
            uint32_t j = 1;
            for (; j < textLen; j++)
            {
                if (s->text[i + j] != text[j]) {
                    i += j;
                    j = INVALID;
                    break;
                }
            }
            if (j != INVALID) { 
                contains = true;
                break;
            };
        }
    }
    return contains;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  STRING CHAR POINTER

uint32_t weu_string_getPointerPos(const weu_string *s) {
    if (s == NULL) return -1;
    return s->charPtrPos;
}
void weu_string_setPointerPos(weu_string *s, uint32_t pos) {
    if (s == NULL) return;
    pos = pos > s->length ? s->length : pos;
    s->charPtrPos = pos;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  FILL
/////////////////////////////////////////////////////////////////////////////////////////////////////

void weu_string_fill(weu_string *s, char fillChar, uint32_t from, uint32_t to) {
    if (s == NULL) return;
    if (from > to) { uint32_t temp; SWAPVAR(from, to, temp); }
    if (to > s->length) {
        weu_string_resize(s, to, ' ');
    }
    memset(&s->text[from], fillChar, to - from);
}
weu_stringNA weu_stringNA_fill(weu_stringNA s, char fillChar, uint32_t from, uint32_t to) {
    if (from > to) { uint32_t temp; SWAPVAR(from, to, temp); }
    weu_stringNA out = s;
    memset(&out.text[from], fillChar, to - from);
    if (to > out.length) out.length = to + 1;
    out.text[out.length] = '\0';
    return out; 
}

weu_string *weu_string_filled(char fillChar, uint32_t len) {
    weu_string *out = malloc(sizeof(weu_string));
    out->length = len + 1;
    out->text = (char*)malloc(len);
    memset(out->text, fillChar, len);
    out->text[out->length] = '\0';
    out->charPtrPos = 0;
    return out;
}
weu_stringNA weu_stringNA_filled(char fillChar, uint32_t len) {
    weu_stringNA out;
    SETLENRANGE(len, 0, 511);
    out.length = len;
    memset(out.text, fillChar, len);
    out.text[len] = '\0';
    return out;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  CONACATE
/////////////////////////////////////////////////////////////////////////////////////////////////////

void weu_string_concateString(weu_string *s, uint8_t count, ...) {
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
void weu_string_concateStringNA(weu_string *s, uint8_t count, ...) {
    if (s == NULL) return;
    va_list args;
    va_start(args, count);
    for (int i = 0; i < count; i++)
    {
        uint32_t oldLen = s->length;
        weu_stringNA str = va_arg(args, weu_stringNA);
        weu_string_resize(s, s->length + str.length, ' ');
        memcpy(s->text + oldLen, str.text, str.length);
    }
    va_end(args);
}
void weu_string_concateText(weu_string *s, uint8_t count, ...) {
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

weu_stringNA weu_stringNA_concatedString(uint8_t count, ...) {
    weu_stringNA out = weu_stringNA_new("");
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
weu_stringNA weu_stringNA_concatedStringNA(uint8_t count, ...) {
    weu_stringNA out = weu_stringNA_new("");
    va_list args;
    va_start(args, count);
    for (int i = 0; i < count; i++)
    {
        if (out.length >= 511) {
            break;
        }
        weu_stringNA str = va_arg(args, weu_stringNA);
        int oldLen = out.length;
        int strLen = str.length;
        strLen = oldLen + strLen > 511 ? 511 - oldLen : strLen;
        out.length = oldLen + strLen;
        memcpy(out.text + oldLen, str.text, strLen);
    }
    va_end(args);
    out.text[out.length] = '\0';
    return out;
}
weu_stringNA weu_stringNA_concatedText(uint8_t count, ...) {
    weu_stringNA out = weu_stringNA_new("");
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

weu_string *weu_string_getLine(const weu_string *s) {
    if (s == NULL) return NULL;
    uint32_t charPointer = s->charPtrPos;
    while (charPointer < s->length) {
        if (s->text[charPointer] == '\n' || s->text[charPointer] == '\0') break;
        ++charPointer;
    }
    weu_string *out = weu_string_fromTo(s, s->charPtrPos, charPointer);
    ((weu_string*)s)->charPtrPos = ++charPointer;
    return out;
}
weu_stringNA weu_string_getLineNA(const weu_string *s) {
    if (s == NULL) return (weu_stringNA){.length = 0, .text = ""};
    uint32_t charPointer = s->charPtrPos;
    while (charPointer < s->length) {
        if (s->text[charPointer] == '\n' || s->text[charPointer] == '\0') break;
        ++charPointer;
    }
    weu_stringNA out = weu_string_fromToNA(s, s->charPtrPos, charPointer);
    ((weu_string*)s)->charPtrPos = ++charPointer;
    return out;
}

weu_string *weu_string_cutLine(weu_string *s) {
    if (s == NULL) return NULL;
    uint32_t isNewLine = 0;
    uint32_t charPointer = 0;
    while (charPointer < s->length) {
        if (s->text[charPointer] == '\n') {
            isNewLine = 1;
            break;
        }
        else if (s->text[charPointer] == '\0') break;
        ++charPointer;
    }
    weu_string *out = weu_string_fromTo(s, 0, charPointer);
    weu_string_removeFromTo(s, 0, charPointer + isNewLine);
    return out;
}
weu_stringNA weu_string_cutLineNA(weu_string *s) {
    if (s == NULL) return (weu_stringNA){.length = 0, .text = ""};
    uint32_t isNewLine = 0;
    uint32_t charPointer = 0;
    while (charPointer < s->length) {
        if (s->text[charPointer] == '\n') {
            isNewLine = 1;
            break;
        }
        else if (s->text[charPointer] == '\0') break;
        ++charPointer;
    }
    weu_stringNA out = weu_string_fromToNA(s, 0, charPointer);
    weu_string_removeFromTo(s, 0, charPointer + isNewLine);
    return out;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  SPLIT
/////////////////////////////////////////////////////////////////////////////////////////////////////

weu_list *weu_string_splitByChar(const weu_string *s, char c) {
    if (s == NULL) return NULL;
    weu_list *out = weu_list_new(0, weu_string_listFree);
    uint32_t sbeg = 0;
    for (uint32_t i = 0; i < s->length; i++) {
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
    uint32_t textLen = strlen(text);
    uint32_t sbeg = 0;
    for (uint32_t i = 0; i < s->length; i++) {
        uint32_t match = 0;
        for (uint32_t j = 0; j < textLen; j++) {
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
//  CHAR REMOVAL
/////////////////////////////////////////////////////////////////////////////////////////////////////

void weu_string_removeChars(weu_string *s, char charToRemove) {
    if (s == NULL) return;
    uint32_t newLen = 0;
    for (uint32_t i = 0; i < s->length; i++)
    {
        if (s->text[i] != charToRemove) {
            s->text[newLen++] = s->text[i];
        }
    }
    s->text[newLen] = '\0';
    s->length = newLen;
}
void weu_string_removeCharsFromBeg(weu_string *s, char charToRemove) {
    if (s == NULL) return;
    uint32_t strStart = 0;
    for (uint32_t i = 0; i < s->length; i++)
    {
        if (s->text[i] != charToRemove) {
            break;
        }
        ++strStart;
    }
    memmove(s->text, s->text + strStart, s->length - strStart);
    s->length = s->length - strStart;
    s->text[s->length] = '\0';
}
void weu_string_removeCharsFromEnd(weu_string *s, char charToRemove) {
    if (s == NULL) return;
    uint32_t newLen = s->length;
    for (int32_t i = s->length - 1; i >= 0; i--)
    {
        if (s->text[i] != charToRemove) {
            break;
        }
        --newLen;
    }
    s->text[newLen] = '\0';
    s->length = newLen;
}

weu_string *weu_string_removedChars(const weu_string *s, char charToRemove) {
    if (s == NULL) return NULL;
    weu_string *out = weu_string_copy(s);
    weu_string_removeChars(out, charToRemove);
    return out;
}
weu_string *weu_string_removedCharsFromBeg(const weu_string *s, char charToRemove) {
    if (s == NULL) return NULL;
    weu_string *out = weu_string_copy(s);
    weu_string_removeCharsFromBeg(out, charToRemove);
    return out;
}
weu_string *weu_string_removedCharsFromEnd(const weu_string *s, char charToRemove) {
    if (s == NULL) return NULL;
    weu_string *out = weu_string_copy(s);
    weu_string_removeCharsFromEnd(out, charToRemove);
    return out;
}

weu_stringNA weu_stringNA_removeChars(weu_stringNA s, char charToRemove) {
    uint32_t newLen = 0;
    for (uint32_t i = 0; i < s.length; i++)
    {
        if (s.text[i] != charToRemove) {
            s.text[newLen++] = s.text[i];
        }
    }
    s.text[newLen] = '\0';
    s.length = newLen;
    return s;
}
weu_stringNA weu_stringNA_removeCharsFromBeg(weu_stringNA s, char charToRemove) {
    uint32_t strStart = 0;
    for (uint32_t i = 0; i < s.length; i++)
    {
        if (s.text[i] != charToRemove) {
            break;
        }
        ++strStart;
    }
    memmove(s.text, s.text + strStart, s.length - strStart);
    s.length = s.length - strStart;
    s.text[s.length] = '\0';
    return s;
}
weu_stringNA weu_stringNA_removeCharsFromEnd(weu_stringNA s, char charToRemove) {
    uint32_t newLen = s.length;
    for (int32_t i = s.length - 1; i >= 0; i--)
    {
        if (s.text[i] != charToRemove) {
            break;
        }
        --newLen;
    }
    s.text[newLen] = '\0';
    s.length = newLen;
    return s;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  INDENTATION
/////////////////////////////////////////////////////////////////////////////////////////////////////

void weu_string_addIndent(weu_string *s, uint8_t count, uint8_t spaceCount) {
    if (s == NULL) return;
    uint32_t tabLen = count * spaceCount;
    uint32_t finalLength = s->length + tabLen;
    if (finalLength > s->allocLength) {
        weu_string_resize(s, finalLength, ' ');
    }
    memmove(&s->text[tabLen], &s->text[0], s->length);
    memset(&s->text[0], ' ', tabLen);   
}
weu_string *weu_string_addedIndent(const weu_string *s, uint8_t count, uint8_t spaceCount) {
    if (s == NULL) return NULL;
    uint32_t tabLen = count * spaceCount;
    uint32_t finalLength = s->length + tabLen;
    weu_string *out = weu_string_newSize(finalLength);
    memcpy(&out->text[tabLen], s->text, s->length);
    memset(out->text, ' ', tabLen);
    return out;
}

void weu_string_removeIndent(weu_string *s) {
    if (s == NULL) return;
    uint32_t offset = 0;
    while (s->text[offset] == ' ' && offset < s->length)
    {
        ++offset;    
    }
    if (offset == 0) return;
    memmove(s->text, &s->text[offset], s->length - offset);
    s->length = s->length - offset;
    s->text[s->length] = '\0';
}
weu_string *weu_string_removedIndent(const weu_string *s) {
    if (s == NULL) return NULL;
    uint32_t offset = 0;
    while (s->text[offset] == ' ' && offset < s->length)
    {
        ++offset;    
    }
    weu_string *out = weu_string_newSize(s->length - offset);
    memcpy(out->text, &s->text[offset], out->length);
    return out;
}
weu_stringNA weu_string_removedIndentNA(const weu_string *s) {
    if (s == NULL) return weu_stringNA_new("");
    uint32_t offset = 0;
    while (s->text[offset] == ' ' && offset < s->length)
    {
        ++offset;    
    }
    weu_stringNA out = weu_stringNA_newSize(s->length - offset);
    memcpy(out.text, &s->text[offset], out.length);
    return out;
}
weu_stringNA weu_stringNA_removeIndent(const weu_stringNA s) {
    uint32_t offset = 0;
    while (s.text[offset] == ' ' && offset < s.length)
    {
        ++offset;    
    }
    weu_stringNA out = weu_stringNA_newSize(s.length - offset);
    memcpy(out.text, &s.text[offset], out.length);
    return out;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  EXPRESSION
/////////////////////////////////////////////////////////////////////////////////////////////////////

bool weu_string_textMatchesExpression(const char *text, const char *expression, weu_list *varyingStringOut, weu_bitfield_32 *isMatching) {
    if (text == NULL || expression == NULL) return false;

    uint32_t textLen    = strlen(text);
    uint32_t exprLen    = strlen(expression);
    bool match          = true;

    uint32_t txtPos = 0;
    uint32_t expPos = 0;

    uint32_t varyCount  = 0;
    weu_bitfield_32 bf  = 0;

    while (txtPos < textLen)
    {
        if (expPos >= exprLen) { return false; }
        if (text[txtPos] == expression[expPos]) {
            ++txtPos;
            ++expPos;
            continue;
        }
        if (expression[expPos] != '%') {
            match = false;
            ++txtPos;
            ++expPos;
            continue;
        }
        if (expression[expPos] == '%') {
            ++expPos;
            //  READ COUNT
            uint32_t readCount = 1;
            if (expression[expPos] >= '0' && expression[expPos] <= '9') {
                readCount = expression[expPos] - '0';
                if (readCount == 0) readCount = 1;
                ++expPos;
            }
            //  TEST CONTEXT
            bool charTest = false;
            if      (expression[expPos] == 'c') charTest = true;
            else if (expression[expPos] == 's') charTest = false;
            else {
                if (varyingStringOut) weu_list_push(varyingStringOut, weu_string_newChar(text[txtPos])); 
                match = false;
                break; 
            }
            ++expPos;
            //  STRING READ END CONDITION
            uint8_t strEnd[6] = {0};
            uint8_t endCount = 0;
            if (expression[expPos] == '{') {
                ++expPos;
                while (expression[expPos] != '}' && expression[expPos] != '\0') {
                    if (endCount < 5) {
                        strEnd[endCount + 1] = expression[expPos];
                        ++endCount;
                    }
                    ++expPos;
                }
                ++expPos;
            }
            endCount += 1;
            //  CONDITION
            weu_stringNA condition = weu_stringNA_new("");
            if (expression[expPos] == '[') {
                uint32_t startPos = expPos++;
                while (expression[expPos] != ']' && expression[expPos] != '\0') {
                    ++expPos;
                }
                condition = weu_stringNA_textFromTo(expression, startPos, ++expPos);
            }
            //  TEST
            for (uint32_t i = 0; i < readCount; i++)
            {
                if (txtPos >= textLen) {
                    match = false;
                    weu_list_push(varyingStringOut, weu_string_new(""));
                    ++varyCount;
                    continue;
                }

                if (charTest) {
                    if (varyingStringOut) weu_list_push(varyingStringOut, weu_string_newChar(text[txtPos]));
                    if (!weu_string_charMatchesCondition(text[txtPos], condition.text)) {
                        match = false;
                    }
                    else SET_BIT32(bf, varyCount);
                    ++varyCount;
                }
                else {
                    // STRING LENGTH
                    uint32_t startPos = txtPos++;
                    bool hitEndCh = false;
                    while (!hitEndCh) {
                        if (txtPos >= textLen) { break; }
                        for (uint32_t i = 0; i < endCount; i++)
                        {
                            if (text[txtPos] == strEnd[i]) { hitEndCh = true; break; }
                        }
                        if (hitEndCh) break;
                        ++txtPos;
                    }

                    weu_stringNA str = weu_stringNA_textFromTo(text, startPos, txtPos);
                    //  Skip over string end char
                    if (hitEndCh) ++expPos;
                    
                    if (varyingStringOut) weu_list_push(varyingStringOut, weu_string_new(str.text));
                    if (!weu_string_textMatchesCondition(str.text, condition.text)) {
                        match = false;
                    }
                    else SET_BIT32(bf, varyCount);
                    ++varyCount;
                }
                if (txtPos < textLen) ++txtPos;
            }
        }
    }
    if (isMatching) *isMatching = bf;
    if (txtPos == textLen && expPos < exprLen) return false;
    return match;
}
bool weu_string_charMatchesCondition(const uint8_t c, const char *condition) {
    if (condition == NULL) return false;

    uint32_t condLen = strlen(condition);
    if (condLen == 0) return true;

    bool testInclusion  = false;
    bool inclusionValid = false;
    bool testExclusion  = false;
    bool exceptionInvalid = false;
    for (uint32_t cPtr = condition[0] == '[' ? 1 : 0; cPtr < condLen;) {
        //  TEST IS CHAR NOT IN RANGE
        if (condition[cPtr] == ']' || condition[cPtr] == '\0') break;
        if (condition[cPtr] == '!' && condition[cPtr + 2] == '-' && !exceptionInvalid) {
            testExclusion = true;
            uint8_t from    = condition[cPtr + 1];
            uint8_t to      = condition[cPtr + 3];
            if (from > to) {uint8_t temp; SWAPVAR(from, to, temp); }
            if (c >= from && c <= to) {
                exceptionInvalid = true;
            }
            cPtr += 4;
        }
        //  TEST IS CHAR IN RANGE
        else if (condition[cPtr + 1] == '-' && !inclusionValid) {
            testInclusion = true;
            uint8_t from    = condition[cPtr];
            uint8_t to      = condition[cPtr + 2];
            if (from > to) {uint8_t temp; SWAPVAR(from, to, temp); }
            if (c >= from && c <= to) {
                inclusionValid = true;
            }
            cPtr += 3;
        }
        //  TEST IS NOT CHAR
        else if (condition[cPtr] == '!' && condition[cPtr + 2] != '-' && !exceptionInvalid) {
            testExclusion = true;
            ++cPtr;
            while (condition[cPtr] != '!' && condition[cPtr + 1] != '-' && condition[cPtr] != ']' && condition[cPtr] != '\0') {
                if (c == condition[cPtr]) {
                    exceptionInvalid = true;
                    break;
                }
                ++cPtr;
            }
            ++cPtr;
        }
        //  TEST IS CHAR
        else if (cPtr > 0 && condition[cPtr - 1] != '-' && condition[cPtr + 1] != '-' && !inclusionValid) {
            testInclusion = true;
            while (condition[cPtr] != '!' && condition[cPtr + 1] != '-' && condition[cPtr] != ']' && condition[cPtr] != '\0') {
                if (c == condition[cPtr]) {
                    inclusionValid = true;
                    break;
                }
                ++cPtr;
            }
            ++cPtr;
        }
        else {
            ++cPtr;
        }
    }
    if ((testInclusion && !inclusionValid) || (testExclusion && exceptionInvalid)) return false;
    else return true;
}
bool weu_string_textMatchesCondition(const char *text, const char *condition) {
    if (text == NULL || condition == NULL) return false;
    uint32_t textLen = strlen(text);
    for (uint32_t i = 0; i < textLen; i++)
    {
        if (!weu_string_charMatchesCondition(text[i], condition)) { return false; }
    }
    return true;
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
    return weu_stringNA_new(buffer);
}
weu_stringNA weu_string_intNA(int32_t val) {
    char buffer[64];
    sprintf(buffer, "%i", val);
    return weu_stringNA_new(buffer);
}
weu_stringNA weu_string_uintNA(uint32_t val) {
    char buffer[64];
    sprintf(buffer, "%u", val);
    return weu_stringNA_new(buffer);
}
weu_stringNA weu_string_llongNA(int64_t val) {
    char buffer[64];
    sprintf(buffer, "%ld", val);
    return weu_stringNA_new(buffer);
}
weu_stringNA weu_string_ullongNA(uint64_t val) {
    char buffer[64];
    sprintf(buffer, "%lu", val);
    return weu_stringNA_new(buffer);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  DEBUG
/////////////////////////////////////////////////////////////////////////////////////////////////////

void weu_string_printText(int count, ...) {
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