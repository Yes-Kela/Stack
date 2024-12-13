#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
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
    if (stk == NULL)
    {
        fprintf(stderr, "StackConstructor is failed\n");
        return STACK_BAD_PTR;
    }

    stk->data = (StackElem_t*) calloc((size_t)capacity, sizeof(StackElem_t));
    if (stk->data == NULL)
    {
        return MEM_ALLOC_ERROR;
    }
    stk->size = 0;
    stk->capacity = capacity;

    STACK_ASSERT_END(stk);
    return STACK_CTOR_SUCCESS;
}


Errors StackPush(Stack_t* stk, StackElem_t value)
{
    STACK_ASSERT_START(stk);

    if (stk->size >= stk->capacity)
    {
        stk->capacity *= 2;
        if (stk->data == NULL)
        {
            return DATA_BAD_PTR;
        }
        stk->data = (StackElem_t*) realloc(stk->data, (size_t)stk->capacity * sizeof(StackElem_t));
        if (stk->data == NULL)
        {
            return MEM_REALLOC_ERROR;
        }
    }

    stk->data[stk->size] = value;
    (stk->size)++;

    STACK_ASSERT_END(stk);

    return STACK_PUSH_SUCCESS;
}

Errors StackPop(Stack_t* stk, StackElem_t* value)
{
    STACK_ASSERT_START(stk);

    if (stk->size == 0)
    {
        return EMPTY_STACK;
    }
    if (stk->size >= MAXSIZE)
    {
        return STACK_BAD_SIZE;
    }

    (stk->size)--;
    *value = stk->data[stk->size];
    stk->data[stk->size] = SPOILED;

    STACK_ASSERT_END(stk);

    return STACK_POP_SUCCESS;
}

int StackVerify(const Stack_t* stk)
{
    // TODO check if all values before size are not spoiled, after - are spoiled
    int error = 0;

    if (stk == NULL)
    {
        error |= STACK_BAD_PTR;
        return error;
    }
    if (stk->data == NULL)
    {
        error |= DATA_BAD_PTR;
    }
    if (stk->size > MAXSIZE)
    {
        error |= STACK_BAD_SIZE;
    }
    if (stk->capacity > MAXSIZE)
    {
        error |= STACK_BAD_CAP;
    }
    if (stk->capacity < stk->size)
    {
        error |= STACK_OVERFLOW;
    }

    /*
    int spoiled_before = 0;
    for (StackSize_t i = 0; i < stk->size; i++)
    {
        if (stk->data[i] == SPOILED)
        {
            spoiled_before = 1;
            break;
        }
    }
    for (StackSize_t i = 0; i < stk->size; i++)
    {
        if (stk->data[i] != SPOILED)
        {

        }
    }
    */

    return error;
}

void StackDump(const Stack_t* stk, const int errnum, const char* file_name, const char* func_name, const int line, const char* mode)
{
    fprintf(stderr, "called from %s: %s(): %d\n", file_name, func_name, line);

    for (size_t position = 0; position < 8 * sizeof(int); position++)
    {
        int cur_error = errnum & (1 << position);
        if (cur_error != 0)
        {
                fprintf(stderr, "%s\n", ErrorNames[errnum]);
        }
    }

    if (errnum == 0 && !strcmp(mode, "beginning"))
    {
        fprintf(stderr, "%s successfully started\n", func_name);
    }
    else if (errnum == 0 && !strcmp(mode, "ending"))
    {
        fprintf(stderr, "%s successfully done\n", func_name);
    }

    fprintf(stderr, "stk [%p]\n", stk);
    if (stk != NULL)
    {
        fprintf(stderr, "size     = %u\n", stk->size);
        fprintf(stderr, "capacity = %u\n", stk->capacity);
        fprintf(stderr, "data [%p] ", stk->data);
        if (stk->data != NULL)
        {
            fprintf(stderr, "=  \n{\n");
            for (StackSize_t i = 0; i < stk->capacity; i++)
            {
                fprintf(stderr, "   *[%u] = %g\n", i, stk->data[i]);            // TODO to use canaries, have to make data - char* and use memcpy for all functions for stack usage; then can use canaries with type that you want
            }
        }
        else
        {
            fprintf(stderr, "\n");
        }
        fprintf(stderr, "}\n\n");
    }
}

Errors StackDestructor(Stack_t* stk)
{
    STACK_ASSERT_START(stk);

    free(stk->data);
    stk->size = MAXSIZE;
    stk->capacity = MAXSIZE;
    stk->data = NULL;

    fprintf(stderr, "called from %s: %s(): %d\n", __FILE__, __func__, __LINE__);
    fprintf(stderr, "%s successfully done\n", __func__);
    fprintf(stderr, "stk [%p]\n", stk);
    if (stk == NULL)
    {
        fprintf(stderr, "%s\n\n", ErrorNames[STACK_BAD_PTR]);
    }
    else
    {
        fprintf(stderr, "size     = %u\n", stk->size);
        fprintf(stderr, "capacity = %u\n", stk->capacity);
        fprintf(stderr, "data [%p] \n", stk->data);
    }

    return STACK_DTOR_SUCCESS;
}

int StackAssert(const Stack_t* stk, const char* file_name, const char* func_name, const int line, const char* mode)
{
    int errnum = StackVerify(stk);
    StackDump(stk, errnum, file_name, func_name, line, mode);

    return errnum;
}

/*
StackSize_t StackGetSize(const Stack_t* stk)
{
    return (stk->size) - 1;
}
*/
