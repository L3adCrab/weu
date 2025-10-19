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
*////////////////////////////////////////////////////////////////////////////////////

#ifndef weu_event_h
#define weu_event_h

#ifndef WEUDEF
    #ifdef WEU_STATIC
        #define WEUDEF static
        #define WEU_IMPLEMENTATION
    #else
        #define WEUDEF extern
    #endif
#endif

#include <stdlib.h>
#include <stdbool.h>

typedef void (*eventCallbackFun)(void*, void*);
typedef struct weu_eventHandler {} weu_eventHandler;

WEUDEF weu_eventHandler *weu_eventHandler_new(int maxSubsriberCount);
WEUDEF void weu_eventHandler_free(weu_eventHandler **h);

WEUDEF void weu_eventHandler_subscribe(weu_eventHandler *h, void *object, eventCallbackFun callback);
WEUDEF void weu_eventHandler_unsubscribe(weu_eventHandler *h, void *obj, eventCallbackFun callback);

WEUDEF void weu_eventHandler_invoke(weu_eventHandler *h, void *data);

#ifdef WEU_IMPLEMENTATION

typedef struct _weu_eventHandler {
    int maxSubscribers, subscriberCount;
    void **subscriberObj;
    eventCallbackFun *eventCallbacks;
} _weu_eventHandler;

weu_eventHandler *weu_eventHandler_new(int maxSubsriberCount) {
    _weu_eventHandler *out = (_weu_eventHandler*)calloc(1, sizeof(_weu_eventHandler));
    out->maxSubscribers     = maxSubsriberCount;
    out->subscriberCount    = 0;
    out->subscriberObj      = (void**)calloc(maxSubsriberCount, sizeof(void*));
    out->eventCallbacks     = (eventCallbackFun*)calloc(maxSubsriberCount, sizeof(eventCallbackFun));
    return (weu_eventHandler*)out;
}
void weu_eventHandler_free(weu_eventHandler **h) {
    if (*h == NULL) return;
    free(((_weu_eventHandler*)*h)->subscriberObj);
    free(((_weu_eventHandler*)*h)->eventCallbacks);
    free(*h);
    *h = NULL;
}

void weu_eventHandler_subscribe(weu_eventHandler *h, void *object, eventCallbackFun callback) {
    _weu_eventHandler *handler = (_weu_eventHandler*)h;
    if (handler == NULL) return;
    if (handler->subscriberCount >= handler->maxSubscribers || callback == NULL) return;
    handler->subscriberObj[handler->subscriberCount] = object;
    handler->eventCallbacks[handler->subscriberCount++] = callback;
}

void weu_eventHandler_unsubscribe(weu_eventHandler *h, void *obj, eventCallbackFun callback) {
    _weu_eventHandler *handler = (_weu_eventHandler*)h;
    if (handler == NULL || callback == NULL) return;
    bool found = false;
    for (int i = 0; i < handler->subscriberCount; i++)
    {
        if (handler->subscriberObj[i] == obj || handler->eventCallbacks[i] == callback) {
            found = true;
        }
        if (found && i < handler->subscriberCount - 1) {
            handler->subscriberObj[i]     = handler->subscriberObj[i + 1];
            handler->eventCallbacks[i]    = handler->eventCallbacks[i + 1];
        }
    }
    if (found) {
        handler->subscriberObj[handler->subscriberCount]    = NULL; 
        handler->eventCallbacks[handler->subscriberCount--] = NULL;
    }
}

void weu_eventHandler_invoke(weu_eventHandler *h, void *data) {
    _weu_eventHandler *handler = (_weu_eventHandler*)h;
    if (handler == NULL) return;
    for (int i = 0; i < handler->subscriberCount; i++)
    {
        handler->eventCallbacks[i](handler->subscriberObj[i], data);
    }
}

#endif
#endif