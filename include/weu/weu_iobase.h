#ifndef weu_iobase_h
#define weu_iobase_h

/*///////////////////////////////////////////////////////////////////////////////////
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