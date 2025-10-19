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

#ifndef weu_coroutine_h
#define weu_coroutine_h

#ifndef WEUCORDEF
#define WEUCORDEF extern
#endif

#include <stdbool.h>

typedef struct weu_coroutine {} weu_coroutine;
typedef int (*enumeratorFun)(int, void*);

WEUCORDEF bool weu_coroutineManager_initialize(int maxCoroutineCount);
WEUCORDEF bool weu_coroutineManager_terminate(void);

WEUCORDEF bool weu_coroutineManager_iterate(void);

WEUCORDEF bool weu_coroutineManager_active(void);
WEUCORDEF int weu_coroutineManager_activeCount(void);

WEUCORDEF weu_coroutine *weu_coroutine_new(enumeratorFun coroutineFunction, int lastPosition, void* passData, bool freeOnDone);
WEUCORDEF bool weu_coroutine_free(weu_coroutine **cr);

WEUCORDEF bool weu_coroutine_start(weu_coroutine **cr);
WEUCORDEF bool weu_coroutine_stop(weu_coroutine *cr);

WEUCORDEF int weu_coroutine_currentID(void);
WEUCORDEF void weu_coroutine_stopCurrent(void);

WEUCORDEF int weu_yield_return_now(void);
WEUCORDEF int weu_yield_return_waitForSeconds(double delay);
WEUCORDEF int weu_yield_return_while(bool condition);

#ifdef WEU_IMPLEMENTATION

#ifdef WEU_COR_DEBUG
#include <stdio.h>
#define ERR_PRINT(X) printf("%s", X)
#else
#define ERR_PRINT(X)
#endif

#include <stdlib.h>
#include <time.h>

typedef struct _weu_coroutine {
    bool active;
    int ID;
    int currentPos, lastPos;
    double timer;
    void *passData;
    enumeratorFun coroutineFun;
    bool freeOnDone;
} _weu_coroutine;

typedef struct _weu_coroutineManager {
    bool active;
    int currentCount, maxCount;
    _weu_coroutine **coroutine;
    weu_coroutine ***coroutineRefs;
} _weu_coroutineManager;

static _weu_coroutineManager _coroutineManager  = {0};
static _weu_coroutine *_currentCoroutine        = NULL;

static time_t _coroutineT1 = 0, _coroutineT2 = 0;
static double _coroutineTimeDelta = 0;

int _weu_coroutineManager_findFreeID(void);
void _weu_coroutine_stop(_weu_coroutine *coroutine);
void _weu_coroutine_stopAll(void);

bool weu_coroutineManager_initialize(int maxCoroutineCount) {
    if (_coroutineManager.active)  {
        ERR_PRINT("weu_coroutineManager_initialize - coroutine manager already running!!\n");
        return false;
    };
    _coroutineManager.currentCount  = 0;
    _coroutineManager.maxCount      = maxCoroutineCount;
    _coroutineManager.coroutine     = (_weu_coroutine**)calloc(maxCoroutineCount, sizeof(_weu_coroutine*));
    _coroutineManager.coroutineRefs = (weu_coroutine***)calloc(maxCoroutineCount, sizeof(weu_coroutine**));
    if (_coroutineManager.coroutine == NULL || _coroutineManager.coroutineRefs == NULL)  {
        ERR_PRINT("weu_coroutineManager_initialize - failed to allocate memory!!\n");
        return false;
    };
    _coroutineManager.active        = true;
    return true;
}
bool weu_coroutineManager_terminate(void) {
    if (_coroutineManager.active == false) {
        ERR_PRINT("weu_coroutineManager_terminate - coroutine manager not initalized!!\n");
        return false;
    };
    _weu_coroutine_stopAll();
    _coroutineManager.active = false;
    free(_coroutineManager.coroutine);
    free(_coroutineManager.coroutineRefs);
    return true;
}

bool weu_coroutineManager_iterate(void) {
    if (!_coroutineManager.active) return false;

    time(&_coroutineT1);
    _coroutineTimeDelta = difftime(_coroutineT1, _coroutineT2);
    _coroutineT2 = _coroutineT1;

    for (int i = 0; i < _coroutineManager.maxCount; i++) {
        _currentCoroutine = _coroutineManager.coroutine[i];
        if (_currentCoroutine == NULL) continue;
        if (!_currentCoroutine->active) continue; 
        if (_currentCoroutine->coroutineFun(_currentCoroutine->currentPos, _currentCoroutine->passData) > _currentCoroutine->lastPos) {
            _weu_coroutine_stop(_currentCoroutine);
        }
    }
    return true;
}

bool weu_coroutineManager_active(void) {
    return _coroutineManager.active;
}
int weu_coroutineManager_activeCount(void) {
    return _coroutineManager.currentCount;
}

weu_coroutine *weu_coroutine_new(enumeratorFun coroutineFunction, int lastPosition, void* passData, bool freeOnDone) {
    _weu_coroutine *out = (_weu_coroutine*)malloc(sizeof(_weu_coroutine));
    if (out == NULL) {
        ERR_PRINT("weu_coroutine_new - failed to allocate memory!!\n");
        return 0;
    };
    out->active     = false;
    out->ID         = -1;
    out->currentPos = 0;
    out->lastPos    = lastPosition;
    out->timer      = 0;
    out->passData   = passData;
    out->coroutineFun = coroutineFunction;
    out->freeOnDone = freeOnDone;
    return (weu_coroutine*)out;
}
bool weu_coroutine_free(weu_coroutine **cr) {
    _weu_coroutine *coroutine = (_weu_coroutine*)*cr;
    if (coroutine == NULL) {
        ERR_PRINT("weu_coroutine_free - coroutine is NULL!!\n");
        return false;
    };
    int ID = coroutine->ID;
    if (coroutine->active) {
        --_coroutineManager.currentCount;
        _coroutineManager.coroutine[ID] = NULL;
        *_coroutineManager.coroutineRefs[ID] = NULL;
    }
    free(*cr);
    *cr = NULL;
    return true;
}

bool weu_coroutine_start(weu_coroutine **cr) {
    _weu_coroutine *coroutine = (_weu_coroutine*)*cr;
    if (coroutine == NULL) {
        ERR_PRINT("weu_coroutine_start - coroutine is NULL!!\n");
        return false;
    }
    if (coroutine->active) {
        ERR_PRINT("weu_coroutine_start - coroutine already running!!\n");
        return false;
    }
    int ID = _weu_coroutineManager_findFreeID();
    if (ID == -1) {
        ERR_PRINT("weu_coroutine_start - coroutine manager out of space!!\n");
        return false;
    }
    
    _coroutineManager.currentCount++;
    coroutine->active   = true;
    coroutine->ID       = ID;
    _coroutineManager.coroutineRefs[ID] = cr;
    _coroutineManager.coroutine[ID]     = coroutine;
    return true;
}
bool weu_coroutine_stop(weu_coroutine *cr) {
    if (cr == NULL) {
        ERR_PRINT("weu_coroutine_stop - coroutine is NULL\n");
        return false;
    };
    _weu_coroutine_stop((_weu_coroutine*)cr);
    return true;
}

int weu_coroutine_currentID(void) {
    if (_currentCoroutine == NULL) return -1;
    return _currentCoroutine->ID;
}
void weu_coroutine_stopCurrent(void) {
    _weu_coroutine_stop(_currentCoroutine);
}

int weu_yield_return_now(void) {
    return _currentCoroutine->currentPos++;
}
int weu_yield_return_waitForSeconds(double delay) {
    _currentCoroutine->timer += _coroutineTimeDelta;
    if (_currentCoroutine->timer >= delay) {
        _currentCoroutine->timer = 0;
        return _currentCoroutine->currentPos++;
    }
    return _currentCoroutine->currentPos;
}
int weu_yield_return_while(bool condition) {
    if (!condition) return _currentCoroutine->currentPos++;
    return _currentCoroutine->currentPos;
}

int _weu_coroutineManager_findFreeID(void) {
    if (_coroutineManager.currentCount >= _coroutineManager.maxCount) return -1;
    for (int i = 0; i < _coroutineManager.maxCount; i++)
    {
        if (_coroutineManager.coroutine[i] == NULL) return i;   
    }
    return -1;
}
void _weu_coroutine_stop(_weu_coroutine *coroutine) {
    --_coroutineManager.currentCount;
    int ID = coroutine->ID;
    coroutine->active   = false;
    coroutine->ID       = -1;
    if (coroutine->freeOnDone) {
        free(coroutine);
    }
    if (ID == -1) return;
    _coroutineManager.coroutine[ID] = NULL;
    *_coroutineManager.coroutineRefs[ID] = NULL;
}
void _weu_coroutine_stopAll(void) {
    if (!_coroutineManager.active) return;
    for (int i = 0; i < _coroutineManager.maxCount; i++)
    {
        if (_coroutineManager.coroutine[i] != NULL) _weu_coroutine_stop(_coroutineManager.coroutine[i]);
    }
}

#endif
#endif