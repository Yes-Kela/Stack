#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <stdlib.h>

#include "stack.h"

int main(void)
{
    FILE* err_file = freopen("errors.txt", "w", stderr);
    if (err_file == NULL)
    {
        fprintf(stdout, "An error occured while opening log-file\n");
        fprintf(stdout, "Sorry, we should stop the program...\n");
        return 1;
    }

    Stack_t stk = {};
    StackSize_t capacity = 5;

    if (StackConstructor(&stk, capacity) != STACK_CTOR_SUCCESS)
    {
        StackStop(&stk, err_file);
        return 1;
    }

    if (StackPush(&stk, 10) != STACK_PUSH_SUCCESS)
    {
        StackStop(&stk, err_file);
        return 1;
    }

    StackElem_t value1 = 0;
    if (StackPop(&stk, &value1) != STACK_POP_SUCCESS)
    {
        StackStop(&stk, err_file);
        return 1;
    }

    if (StackPush(&stk, 20) != STACK_PUSH_SUCCESS)
    {
        StackStop(&stk, err_file);
        return 1;
    }

    StackElem_t value = 0;
    if (StackPop(&stk, &value) != STACK_POP_SUCCESS)
    {
        StackStop(&stk, err_file);
        return 1;
    }
    fprintf(stdout, "value = %u\n", value);

    StackDestructor(&stk);
    fclose(err_file);

    return 0;
}
