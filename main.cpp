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
        return 1;
    }

    StackPush(&stk, 10);
    StackPush(&stk, 20);

    StackElem_t value = 0;
    StackPop(&stk, &value);
    fprintf(stdout, "value = %u\n", value);

    StackDestructor(&stk);

    fclose(err_file);

    return 0;
}
