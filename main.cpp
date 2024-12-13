#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <stdlib.h>

#include "stack.h"

int main(void)
{
    Stack_t stk = {};
    StackSize_t capacity = 5;

    if (StackConstructor(&stk, capacity) != STACK_CTOR_SUCCESS)
    {
        return 1;
    }

    StackPush(&stk, 10);
    StackPush(&stk, 20);

    StackElem_t value = 0;
    StackPop(&stk, &value);
    fprintf(stdout, "value = %g\n", value);

    StackDestructor(&stk);

    return 0;
}
