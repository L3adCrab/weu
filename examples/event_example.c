/*  GCC example build command

gcc -Wall -Wextra -Werror -std=c99 -g examples/event_example.c -o a.out

*/

#include <stdlib.h>
#include <stdio.h>
#include <memory.h>

#include <time.h>

#define WEU_IMPLEMENTATION
#include "../include/weu/weu_event.h"

weu_eventHandler *eventHandler;
typedef struct testStruct {
    char text1[64];
    char text2[64];
} testStruct;

void event(void *obj, void *data);

int main() {
    eventHandler = weu_eventHandler_new(10);

    weu_eventHandler_subscribe(eventHandler, NULL, event);

    // events_unsubscribe(eventHandler, NULL, event);

    const char *text1 = "test string";
    const char *text2 = "event\nsystem test";

    testStruct test;
    memcpy(test.text1, text1, strlen(text1));
    memcpy(test.text2, text2, strlen(text2));

    weu_eventHandler_invoke(eventHandler, &test);

    weu_eventHandler_free(&eventHandler);

    return 1;
}

void event(void *obj, void *data) {
    (void)obj;
    testStruct *test = (testStruct*)data;
    printf("%s\n", test->text1);
    printf("%s\n", test->text2);
}