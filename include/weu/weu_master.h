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

#ifndef weu_master_h
#define weu_master_h

#include "weu_bitfield.h"
#include "weu_hashtable.h"
#include "weu_event.h"
#include "weu_iobase.h"
#include "weu_list.h"
#include "weu_pair.h"
#include "weu_string.h"

#endif