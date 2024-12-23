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
    "Stack Overflow",
    "Memory allocarion error",
    "Memory reallocation error",
    "Attempt to pop from empty stack",
    "Unauthorized access to the data from the left",
    "Unauthorized access to the data from the right"
};

Errors StackConstructor(Stack_t* stk, StackSize_t capacity)
{
    if (stk == NULL)
    {
        fprintf(stderr, "StackConstructor() is failed\n");
        return STACK_BAD_PTR;
    }
    if (capacity >= MAXSIZE)
    {
        fprintf(stderr, "Incorrect capacity was passed to the StackConstructor()\n");
        return STACK_BAD_CAP;
    }
    stk->data = (char*) calloc(capacity*sizeof(StackElem_t) + 2*sizeof(StackCanary_t), sizeof(char));
    if (stk->data == NULL)
    {
        return MEM_ALLOC_ERROR;
    }
    stk->size = 0;
    stk->capacity = capacity;

    //StackDump(stk, 0, __FILE__, __func__, __LINE__, "beginning");

    StackCanary_t left_canary = LEFT_CANARY;
    StackCanary_t right_canary = RIGHT_CANARY;
    memcpy(stk->data, &left_canary, sizeof(StackCanary_t));
    memcpy(stk->data + StackRealCapacity(stk), &right_canary, sizeof(StackCanary_t));

    int errnum = STACK_ASSERT_END(stk);
    if (errnum != 0)
    {
        return STACK_CTOR_FAILED;
    }
    return STACK_CTOR_SUCCESS;
}

Errors StackPush(Stack_t* stk, StackElem_t value)
{
    int errnum = STACK_ASSERT_START(stk);
    if (errnum != 0)
    {
        return STACK_PUSH_FAILED;
    }

    if (stk->size >= stk->capacity)
    {
        stk->capacity *= 2;
        if (stk->data == NULL)
        {
            return DATA_BAD_PTR;
        }
        stk->data = (char*) realloc(stk->data, (size_t)stk->capacity * sizeof(StackElem_t) + 2*sizeof(StackCanary_t));
        if (stk->data == NULL)
        {
            return MEM_REALLOC_ERROR;
        }
    }

    memcpy(stk->data + StackRealSize(stk), &value, sizeof(StackCanary_t));
    (stk->size)++;

    errnum = STACK_ASSERT_END(stk);
    if (errnum != 0)
    {
        return STACK_PUSH_FAILED;
    }
    return STACK_PUSH_SUCCESS;
}

Errors StackPop(Stack_t* stk, StackElem_t* value)
{
    int errnum = STACK_ASSERT_START(stk);
    if (errnum != 0)
    {
        return STACK_POP_FAILED;
    }

    if (stk->size == 0)
    {
        return EMPTY_STACK;
    }
    if (stk->size >= MAXSIZE)
    {
        return STACK_BAD_SIZE;
    }

    (stk->size)--;
    memcpy(value, stk->data + StackRealSize(stk), sizeof(StackElem_t));
    StackElem_t spoiled = SPOILED;
    memcpy(stk->data + StackRealSize(stk), &spoiled, sizeof(StackElem_t));

    errnum = STACK_ASSERT_END(stk);
    if (errnum != 0)
    {
        return STACK_POP_FAILED;
    }

    return STACK_POP_SUCCESS;
}

int StackVerify(const Stack_t* stk)
{
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
    if (error != 0)
    {
        return error;
    }

    StackCanary_t left_canary = 0;
    StackCanary_t right_canary = 0;
    memcpy(&left_canary, stk->data, sizeof(StackCanary_t));
    memcpy(&right_canary, stk->data + StackRealCapacity(stk), sizeof(StackCanary_t));

    if (left_canary != LEFT_CANARY)
    {
        error |= LEFT_CANARY_DIED;
    }
    if (right_canary != RIGHT_CANARY)
    {
        error |= RIGHT_CANARY_DIED;
    }

    for (StackSize_t i = 0; i < stk->size; i++)
    {
        StackElem_t elem = 0;
        memcpy(&elem, stk->data + sizeof(StackCanary_t) + i*sizeof(StackElem_t), sizeof(StackElem_t));
        if (elem == 0 || elem == SPOILED)
        {
            error |= ATTACK_FROM_THE_LEFT;
            break;
        }
    }
    for (StackSize_t i = stk->size; i < stk->capacity; i++)
    {
        StackElem_t elem = 0;
        memcpy(&elem, stk->data + sizeof(StackCanary_t) + i*sizeof(StackElem_t), sizeof(StackElem_t));
        if (elem != 0 && elem != SPOILED)
        {
            error |= ATTACK_FROM_THE_RIGHT;
        }
    }

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
        fprintf(stderr, "%s() successfully started\n", func_name);
    }
    else if (errnum == 0 && !strcmp(mode, "ending"))
    {
        fprintf(stderr, "%s() successfully done\n", func_name);
    }

    fprintf(stderr, "{\n");
    fprintf(stderr, "    stk [%p]\n", stk);
    if (stk != NULL)
    {
        fprintf(stderr, "    {\n");
        fprintf(stderr, "        size     = %u\n", stk->size);
        fprintf(stderr, "        capacity = %u\n", stk->capacity);
        fprintf(stderr, "        data [%p] ", stk->data);
        if (stk->data != NULL)
        {
            fprintf(stderr, "=  \n        {\n");

            StackCanary_t left_canary = 0;
            memcpy(&left_canary, stk->data, sizeof(StackCanary_t));
            fprintf(stderr, "            left_canary = 0x%lX\n", left_canary);

            for (StackSize_t i = 0; i < stk->capacity; i++)
            {
                StackElem_t spoiled = 0;
                memcpy(&spoiled, stk->data + sizeof(StackCanary_t) + i*sizeof(StackElem_t), sizeof(StackElem_t));
                if ((StackElem_t) spoiled == (StackElem_t) SPOILED)
                {
                    fprintf(stderr, "            *[%u] = 0x%X\n", i, spoiled);
                }
                else
                {
                    fprintf(stderr, "            *[%u] = %u\n", i, stk->data[sizeof(StackCanary_t) + i*sizeof(StackElem_t)]);
                }
            }

            StackCanary_t right_canary = 0;
            memcpy(&right_canary, stk->data + StackRealCapacity(stk), sizeof(StackCanary_t));
            fprintf(stderr, "            right_canary = 0x%lX\n", right_canary);
            fprintf(stderr, "        }\n");
        }
        else
        {
            fprintf(stderr, "\n");
        }
        fprintf(stderr, "    }\n");
    }
    fprintf(stderr, "}\n\n");
}

Errors StackDestructor(Stack_t* stk)
{
    int errnum = STACK_ASSERT_START(stk);
    if (errnum != 0)
    {
        return STACK_DTOR_FAILED;
    }

    free(stk->data);
    stk->size = MAXSIZE;
    stk->capacity = MAXSIZE;
    stk->data = NULL;

    fprintf(stderr, "called from %s: %s(): %d\n", __FILE__, __func__, __LINE__);
    //fprintf(stderr, "%s() successfully done\n", __func__);
    fprintf(stderr, "{\n    stk [%p]\n", stk);
    if (stk == NULL)
    {
        fprintf(stderr, "   %s\n}\n", ErrorNames[STACK_BAD_PTR]);
    }
    else
    {
        fprintf(stderr, "    {\n");
        fprintf(stderr, "        size     = %u\n", stk->size);
        fprintf(stderr, "        capacity = %u\n", stk->capacity);
        fprintf(stderr, "        data [%p]\n", stk->data);
        fprintf(stderr, "    }\n}\n");
    }
    fprintf(stderr, "\n");

    return STACK_DTOR_SUCCESS;
}

int StackAssert(const Stack_t* stk, const char* file_name, const char* func_name, const int line, const char* mode)
{
    int errnum = StackVerify(stk);
    StackDump(stk, errnum, file_name, func_name, line, mode);

    return errnum;
}

StackSize_t StackRealSize(const Stack_t* stk)
{
    return 1*sizeof(StackCanary_t) + (stk->size)*sizeof(StackElem_t);
}

StackSize_t StackRealCapacity(const Stack_t* stk)
{
    return 1*sizeof(StackCanary_t) + (stk->capacity)*sizeof(StackCanary_t);
}

void StackStop(Stack_t* stk, FILE* file)
{
    StackDestructor(stk);
    fclose(file);
}
