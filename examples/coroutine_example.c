/*  GCC example build command

gcc -Wall -Wextra -Werror -std=c99 -g examples/coroutine_example.c -o a.out

//  
gcc -Wall -Wextra -Werror -DWEU_COR_DEBUG -std=c99 -g examples/coroutine_example.c -o a.out

*/

#define WEU_IMPLEMENTATION
#include "../include/weu/weu_coroutine.h"

#include <stdio.h>

int coroutine(int, void*);
struct coroutineData {int value;} coroutineData;

int main() {
    coroutineData.value = 0;
    weu_coroutineManager_initialize(3);
    for (int i = 0; i < 2; i++)
    {
        weu_coroutine *cr = weu_coroutine_new(coroutine, 2, &(struct coroutineData){i}, true);
        weu_coroutine_start(&cr);

        weu_coroutine *cr1 = weu_coroutine_new(coroutine, 2, &(struct coroutineData){i + 1}, true);
        weu_coroutine_start(&cr1);

    }
    while (weu_coroutineManager_activeCount() > 0)
    {
        weu_coroutineManager_iterate();
    }
    weu_coroutineManager_terminate();
    
}

int coroutine(int position, void *passData) {
    int *value = &((struct coroutineData*)passData)->value;
    switch (position)
    {
    case 0:
        printf("Starting coroutine - %i\n", weu_coroutine_currentID());
        return weu_yield_return_now();
        case 1:
        return weu_yield_return_waitForSeconds(*value);
        case 2:
        printf("Closing coroutine - %i\n", weu_coroutine_currentID());
        return weu_yield_return_now();
    default:
        break;
    }
    return position;
}