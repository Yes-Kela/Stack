#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <stdlib.h>

#include "stack.h"

int main(void)
{
    Stack_t stk = {NULL, 0, 0};
    StackSize_t capacity = 5;

    StackConstructor(&stk, capacity);
    fprintf(stderr, "StackConstructor: %d\n", StackAssert(&stk));

    StackPush(&stk, 100);
    fprintf(stderr, "StackPush: %d\n", StackAssert(&stk));

    StackPush(&stk, 200);
    fprintf(stderr, "StackPush: %d\n", StackAssert(&stk));

    StackPush(&stk, 300);
    fprintf(stderr, "StackPush: %d\n", StackAssert(&stk));

    StackPush(&stk, 400);
    fprintf(stderr, "StackPush: %d\n", StackAssert(&stk));

    StackPush(&stk, 500);
    fprintf(stderr, "StackPush: %d\n", StackAssert(&stk));

    StackPush(&stk, 600);
    fprintf(stderr, "StackPush: %d\n", StackAssert(&stk));

    StackElem_t value = 0;
    StackPop(&stk, &value);
    fprintf(stderr, "StackPop: %d\n", StackAssert(&stk));
    fprintf(stderr, "value = %g\n", value);

    StackPop(&stk, &value);
    fprintf(stderr, "StackPop: %d\n", StackAssert(&stk));
    fprintf(stderr, "value = %g\n", value);

    StackPop(&stk, &value);
    fprintf(stderr, "StackPop: %d\n", StackAssert(&stk));
    fprintf(stderr, "value = %g\n", value);

    StackDestructor(&stk);
    //printf("StackDestructor: %d\n", StackAssert(&stk));

    return 0;
}
