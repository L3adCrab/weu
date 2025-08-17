#ifndef weu_iobase_h
#define weu_iobase_h

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
#include "weu_string.h"

#include <stdio.h>

/////////////////////////////////////////////////////////////////////////////////////////////////////
//  READ

//  returns NULL if file doesn't exist
WEUDEF weu_string *weu_io_loadText(const char *filePath) {
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

WEUDEF void weu_io_writeFile(const char *filePath, const char *text) {
    FILE *file = fopen(filePath, "w");
    if (file == NULL) return;
    fprintf(file, text);
    fclose(file);
}
WEUDEF void weu_io_appendFile(const char *filePath, const char *text) {
    FILE *file = fopen(filePath, "a");
    if (file == NULL) return;
    fprintf(file, text);
    fclose(file);
}

#endif
#endif