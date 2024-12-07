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

// i want functions to return error codes, but must they be Errors-types?
// the problem is mixing the error codes that show real errors and codes from particular functions
// make new enum-constants for errors that show how correct the function works?

Errors StackConstructor(Stack_t* stk, StackSize_t capacity)
{
    //STACK_ASSERT(stk);                                           // actually, bad: the data pointer is null before StackCtor
    if (stk == NULL)
    {
        fprintf(stderr, "StackConstructor is failed\n");
        assert(NULL);
    }

    stk->data = (StackElem_t*) calloc((size_t)capacity, sizeof(StackElem_t));
    stk->size = 0;
    stk->capacity = capacity;

    STACK_ASSERT(stk);
    return STACK_IS_OKAY;                      // what must Ctor return?
}

Errors StackPush(Stack_t* stk, StackElem_t value)
{
    STACK_ASSERT(stk);
    StackDump(stk, 0, __FILE__, __func__, __LINE__);

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

    STACK_ASSERT(stk);
    StackDump(stk, 0, __FILE__, __func__, __LINE__);


    return STACK_IS_OKAY;                        // what must Push return?
}

Errors StackPop(Stack_t* stk, StackElem_t* value)
{
    STACK_ASSERT(stk);

    (stk->size)--;
    *value = stk->data[stk->size];
    stk->data[stk->size] = SPOILED;

    STACK_ASSERT(stk);

    return STACK_IS_OKAY;                            // what must Rop return?
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

    return error;
}

void StackDump(const Stack_t* stk, const int errnum, const char* file_name, const char* func_name, const int line)
{
    fprintf(stderr, "called from %s: %s(): %d\n", file_name, func_name, line);
    fprintf(stderr, "%s\n", ErrorNames[errnum]);
    fprintf(stderr, "stk [%p]\n", stk);
    if (stk == NULL)
    {
        fprintf(stderr, "%s\n", ErrorNames[STACK_BAD_PTR]);
    }
    else
    {
        fprintf(stderr, "size     = %u\n", stk->size);
        fprintf(stderr, "capacity = %u\n", stk->capacity);
        fprintf(stderr, "data [%p] =  \n{\n", stk->data);
        if (stk->data == NULL && !strcmp(file_name, "StackConstructor"))           // but if the calling in the end of StackCtor?
        {
            fprintf(stderr, "   %s\n", ErrorNames[DATA_BAD_PTR]);
        }
        else if (stk->data == NULL)
        {
            fprintf(stderr, "   StackConstructor() is working now\n");
        }
        else
        {
            for (StackSize_t i = 0; i < stk->capacity; i++)
            {
                fprintf(stderr, "   *[%u] = %g\n", i, stk->data[i]);
            }
        }
        fprintf(stderr, "}\n");
    }
}

Errors StackDestructor(Stack_t* stk)
{
    STACK_ASSERT(stk);

    free(stk->data);
    stk->size = MAXSIZE;
    stk->capacity = MAXSIZE;
    stk->data = NULL;

    return STACK_IS_OKAY;                   // what Dtor must return?
}

int StackAssert(const Stack_t* stk, const char* file_name, const char* func_name, const int line)
{
    int verify = StackVerify(stk);
    if (verify != 0)
    {
        for (size_t position = 0; position < 8 * sizeof(int); position++)
        {
            int cur_error = verify & (1 << position);
            if (cur_error != 0)
            {
                StackDump(stk, cur_error, file_name, func_name, line);
            }
            assert(NULL);
        }
    }
    return verify;                          // what Assert must return?
}
