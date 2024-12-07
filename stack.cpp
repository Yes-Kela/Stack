#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <stdlib.h>                             // split into files: cpp and headers
#include "stack.h"

static const char* ErrorNames[] =
{
    "Stack is okay",
    "NULL pointer to stack",
    "NULL pointer to data",
    "Stack size is incorrect",
    "Stack capacity is incorrect",
    "Stack Overflow"
};

Errors StackConstructor(Stack_t* stk, StackSize_t capacity)
{
    //fprintf(stderr, "%s\n", __func__);

    if (stk == NULL)
        return STACK_BAD_PTR;

    stk->data = (StackElem_t*) calloc((size_t)capacity, sizeof(StackElem_t));
    stk->size = 0;
    stk->capacity = capacity;

    StackAssert(stk);
    return STACK_IS_OKAY;
}

Errors StackPush(Stack_t* stk, StackElem_t value)
{
    StackAssert(stk);

    if (stk->size >= stk->capacity)
    {
        stk->capacity *= 2;
        if (stk->data == NULL)
        {
            return DATA_BAD_PTR;
        }
        stk->data = (StackElem_t*) realloc(stk->data, (size_t)stk->capacity * sizeof(StackElem_t));
    }

    stk->data[stk->size] = value;
    (stk->size)++;

    StackAssert(stk);

    return STACK_IS_OKAY;
}

Errors StackPop(Stack_t* stk, StackElem_t* value)
{
    StackAssert(stk);

    (stk->size)--;
    *value = stk->data[stk->size];
    stk->data[stk->size] = SPOILED;

    StackAssert(stk);

    return STACK_IS_OKAY;
}

int StackVerify(const Stack_t* stk)
{
    int error = 0;

    if (stk == NULL)
    {
        error |= STACK_BAD_PTR;
    }
    if (stk->data == NULL)
    {
        error |= DATA_BAD_PTR;
    }
    else if (stk->size > MAXSIZE)
    {
        error |= STACK_BAD_SIZE;
    }
    else if (stk->capacity > MAXSIZE)
    {
        error |= STACK_BAD_CAP;
    }
    else if (stk->capacity < stk->size)
    {
        error |= STACK_OVERFLOW;
    }

    return error;
}

void StackDump(const Stack_t* stk, const int errnum)
{
    fprintf(stderr, "%s\n", ErrorNames[errnum]);
    if (errnum != STACK_IS_OKAY)
    {
        fprintf(stderr, "stk: %p\n", stk);
        fprintf(stderr, "stk.size: %u\n", stk->size);
        fprintf(stderr, "stk.capacity: %u\n", stk->capacity);
        fprintf(stderr, "stk.data: %p\n", stk->data);
        if (stk != NULL)
        {
            fprintf(stderr, " index      value\n");
            for (StackSize_t i = 0; i < stk->capacity; i++)
            {
                fprintf(stderr, "%5u %10g\n", i, stk->data[i]);
            }
        }
        fprintf(stderr, "\n\n");
    }
}

Errors StackDestructor(Stack_t* stk)
{
    free(stk->data);
    stk->size = MAXSIZE;
    stk->capacity = MAXSIZE;
    stk->data = NULL;

    return STACK_IS_OKAY;
}

int StackAssert(const Stack_t* stk)
{
    int verify = StackVerify(stk);
    if (verify == 0)
    {
        StackDump(stk, verify);
        return STACK_IS_OKAY;
    }
    for (size_t position = 0; position < 8 * sizeof(int); position++)
    {
        int cur_error = verify & (1 << position);
        if (cur_error != STACK_IS_OKAY)
        {
            StackDump(stk, cur_error);
        }
    }
    return verify;
}
