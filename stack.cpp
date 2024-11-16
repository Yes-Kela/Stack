#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <stdlib.h>                             // split into files: cpp and headers

#define MAXSIZE SIZE_MAX - 2

typedef double StackElem_t;
typedef size_t StackSize_t;
typedef struct
{
    StackElem_t* data;
    StackSize_t size;
    StackSize_t capacity;
} Stack_t;

enum Errors
{
    STACK_IS_OKAY     = 1 << 0,                  // ???
    STACK_CTOR_FAILED = 1 << 1,                  // Побитовые операции добавить
    STACK_PUSH_FAILED = 1 << 2,
    STACK_POP_FAILED  = 1 << 3,
    STACK_DTOR_FAILED = 4,                       // До этого момента enum-константы бессмысленны!
    STACK_BAD_PTR     = 5,
    DATA_BAD_PTR      = 6,
    STACK_BAD_SIZE    = 7,
    STACK_BAD_CAP     = 8,
    STACK_OVERFLOW    = 9
};

static const char* ErrorNames[] =
{
    "Stack is okay",
    "StackConstructor() failed",
    "StackPush() failed",
    "StackPop() failed",
    "StackDestructor() failed",
    "NULL pointer to stack",
    "NULL pointer to data",
    "Stack size is incorrect",
    "Stack capacity is incorrect",
    "Stack Overflow"
};

int StackConstructor(Stack_t* stk, StackSize_t capacity);
Errors StackPush(Stack_t* stk, StackElem_t value);
int StackPop(Stack_t* stk, StackElem_t* value);

int StackVerify(const Stack_t* stk);
void StackDump(const Stack_t* stk, const int errnum);
int StackAssert(const Stack_t* stk);

int StackDestructor(Stack_t* stk);

int main(void)
{
    Stack_t stk = {};
    StackSize_t capacity = 5;

    StackConstructor(&stk, capacity);
    printf("StackConstructor: %d\n", StackAssert(&stk));

    StackPush(&stk, 100);
    printf("StackPush: %d\n", StackAssert(&stk));

    StackPush(&stk, 200);
    printf("StackPush: %d\n", StackAssert(&stk));

    StackPush(&stk, 300);
    printf("StackPush: %d\n", StackAssert(&stk));

    StackPush(&stk, 400);
    printf("StackPush: %d\n", StackAssert(&stk));

    StackPush(&stk, 500);
    printf("StackPush: %d\n", StackAssert(&stk));

    StackPush(&stk, 600);
    printf("StackPush: %d\n", StackAssert(&stk));

    StackElem_t value = 0;
    StackPop(&stk, &value);
    printf("StackPop: %d\n", StackAssert(&stk));
    printf("value = %g\n", value);

    StackPop(&stk, &value);
    printf("StackPop: %d\n", StackAssert(&stk));
    printf("value = %g\n", value);

    StackPop(&stk, &value);
    printf("StackPop: %d\n", StackAssert(&stk));
    printf("value = %g\n", value);

    StackDestructor(&stk);
    //printf("StackDestructor: %d\n", StackAssert(&stk));

    return 0;
}

int StackConstructor(Stack_t* stk, StackSize_t capacity)
{
    if (stk == NULL)
        return STACK_CTOR_FAILED;

    stk->data = (StackElem_t*) calloc((size_t)capacity, sizeof(StackElem_t));
    stk->size = 0;
    stk->capacity = capacity;

    return StackAssert(stk);
}

int StackPush(Stack_t* stk, StackElem_t value)
{
    if (StackAssert(stk) != STACK_IS_OKAY)
    {
        return STACK_PUSH_FAILED;
    }

    if (stk->size >= stk->capacity)
    {
        stk->capacity *= 2;
        stk->data = (StackElem_t*) realloc(stk->data, (size_t)stk->capacity * sizeof(StackElem_t));     // check error, а вдруг указатель нулевой?
    }

    stk->data[stk->size] = value;
    (stk->size)++;

    if (StackAssert(stk) != STACK_IS_OKAY)
    {
        return STACK_PUSH_FAILED;
    }

    return 0;
}

int StackPop(Stack_t* stk, StackElem_t* value)
{
    if (StackAssert(stk) != STACK_IS_OKAY)
    {
        return STACK_POP_FAILED;
    }

    (stk->size)--;
    *value = stk->data[stk->size];
    stk->data[stk->size] = 0;                     // Make constant!!!

    if (StackAssert(stk) != STACK_IS_OKAY)
    {
        return STACK_POP_FAILED;
    }

    return 0;
}

int StackVerify(const Stack_t* stk)                // Для нескольких  ошибок через побитовые операции
{                                                  // И куда возвращать эти коды? В main()? Или в одну из функций?
    int error = 0;

    if (stk == NULL)
    {
        error |= STACK_BAD_PTR;                 // учитываем каждую ошибку - доделать
        return STACK_BAD_PTR;
    }
    if (stk->data == NULL)
    {
        return DATA_BAD_PTR;
    }
    else if (stk->size > MAXSIZE)
    {
        return STACK_BAD_SIZE;
    }
    else if (stk->capacity > MAXSIZE)
    {
        return STACK_BAD_CAP;
    }
    else if (stk->capacity < stk->size)
    {
        return STACK_OVERFLOW;
    }

    return STACK_IS_OKAY;
}

void StackDump(const Stack_t* stk, const int errnum)                       // И куда печатать дамп? В файлик?
{                                                                          // into stderr!!
    printf("%s\n", ErrorNames[errnum]);
    printf("stk: %p\n", stk);
    printf("stk.size: %u\n", stk->size);
    printf("stk.capacity: %u\n", stk->capacity);
    printf("stk.data: %p\n", stk->data);
    if (stk != NULL)
    {
        printf(" index      value\n");
        for (StackSize_t i = 0; i < stk->capacity; i++)
        {
            printf("%5u %10g\n", i, stk->data[i]);
        }
    }
    printf("\n\n");
}

int StackDestructor(Stack_t* stk)
{
    free(stk->data);
    stk->size = MAXSIZE;
    stk->capacity = MAXSIZE;
    stk->data = NULL;

    return 0;
}

int StackAssert(const Stack_t* stk)                    // Think about splitting returning error code and doing dump!
{
    int verify = StackVerify(stk);
    StackDump(stk, verify);
    return verify;
}
