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
*////////////////////////////////////////////////////////////////////////////////////

#ifndef weu_iobase_h
#define weu_iobase_h

#define WEUDEF extern

#include "weu_datatypes.h"
#include "weu_string.h"

#include <stdio.h>

/////////////////////////////////////////////////////////////////////////////////////////////////////
//  READ

//  returns NULL if file doesn't exist
WEUDEF weu_string *weu_io_loadText(const char *filePath);
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  WRITE

WEUDEF void weu_io_writeFile(const char *filePath, const char *text);
WEUDEF void weu_io_appendFile(const char *filePath, const char *text);

#ifdef WEU_IMPLEMENTATION

/////////////////////////////////////////////////////////////////////////////////////////////////////
//  READ
/////////////////////////////////////////////////////////////////////////////////////////////////////

//  returns NULL if file doesn't exist
weu_string *weu_io_loadText(const char *filePath)
{
    FILE *file = fopen(filePath, "r");
    if (file == NULL) return NULL;

    fseek(file, 0, SEEK_END);
    int charCount = ftell(file);
    rewind(file);

    weu_string *out = weu_string_newSize(charCount);
    for (int i = 0; i < charCount; i++) {
        out->text[i] = getc(file);
    }
    fclose(file);
    return out;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  WRITE
/////////////////////////////////////////////////////////////////////////////////////////////////////

void weu_io_writeFile(const char *filePath, const char *text) {
    FILE *file = fopen(filePath, "w");
    if (file == NULL) return;
    fprintf(file, text);
    fclose(file);
}
void weu_io_appendFile(const char *filePath, const char *text) {
    FILE *file = fopen(filePath, "a");
    if (file == NULL) return;
    fprintf(file, text);
    fclose(file);
}

#endif
#endif