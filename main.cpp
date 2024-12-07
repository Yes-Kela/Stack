#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <stdlib.h>

#include "stack.h"

int main(void)
{
    Stack_t stk = {};
    StackSize_t capacity = 5;

    StackConstructor(&stk, capacity);

    StackPush(&stk, 100);

    StackPush(&stk, 200);

    StackPush(&stk, 300);

    StackPush(&stk, 400);

    StackPush(&stk, 500);

    StackPush(&stk, 600);

    StackElem_t value = 0;
    StackPop(&stk, &value);
    fprintf(stderr, "value = %g\n", value);

    StackPop(&stk, &value);
    fprintf(stderr, "value = %g\n", value);

    StackPop(&stk, &value);
    fprintf(stderr, "value = %g\n", value);

    StackDestructor(&stk);
    //printf("StackDestructor: %d\n", StackAssert(&stk));

    return 0;
}
