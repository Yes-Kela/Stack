#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <stdlib.h>

typedef double StackElem_t;
typedef long long int StackSize_t;
typedef struct
{
    StackElem_t* data;
    StackSize_t size;
    StackSize_t capacity;
} Stack_t;

int StackConstructor(Stack_t* stk, StackSize_t capacity);
int StackPush(Stack_t* stk, StackElem_t value);
int StackPop(Stack_t* stk);
int StackVerify(const Stack_t* stk);
int StackDump(const Stack_t* stk);
//void StackAssert(const Stack_t* stk);
int StackDestructor(Stack_t* stk);

int main(void)
{
    Stack_t stk = {};
    StackSize_t capacity = 5;
    StackConstructor(&stk, capacity);
    StackVerify(&stk);

    StackPush(&stk, 10);
    if (StackVerify(&stk) != 0)
    {
        StackDump(&stk);
        assert(NULL);
    }

    //StackPop();

    StackDestructor(&stk);

    return 0;
}

int StackConstructor(Stack_t* stk, StackSize_t capacity)
{
    if (stk != NULL)
    {
        stk->data = (StackElem_t*) calloc(capacity, sizeof(StackElem_t));
        stk->size = 0;
        stk->capacity = capacity;
        return StackVerify(stk);
    }
    else
    {
        return 1;
    }

    return 0;
}

int StackPush(Stack_t* stk, StackElem_t value)
{
    int verify = StackVerify(stk);
    if (verify != 0)
    {
        StackDump(stk);
        return verify;
    }

    if (stk->size >= stk->capacity)
    {
        (stk->capacity)*= 2;
        stk->data = (StackElem_t*) realloc(stk->data, stk->capacity * sizeof(StackElem_t));
    }

    stk->data[stk->size] = value;
    (stk->size)++;

    verify = StackVerify(stk);
    if (verify != 0)
    {
        StackDump(stk);
        return verify;
    }

    return 0;
}

int StackVerify(const Stack_t* stk)                // А как возвратить коды сразу нескольких ошибок?
{                                                  // И куда возвращать эти коды? В main()? Или в одну из функций? И надо ли фигачить
    if (stk->data == NULL)                         // StackVerify() не в mainе()?
    {
        return 1;              //STACKBADPTR
    }
    else if (stk->size < 0)
    {
        return 2;              //STACKBADSIZE
    }
    else if (stk->capacity < 0)
    {
        return 3;              //STACKBADCAP
    }
    else if (stk->capacity < stk->size)
    {
        return 4;              //STACKOVERFLOW
    }
    return 0;
}

int StackDump(const Stack_t* stk)                       // И куда печатать дамп? В файлик?
{
    printf("Stack_t* stk: %p\n", stk);
    printf("stk.size: %lld\n", stk->size);
    printf("stk.capacity: %lld\n", stk->capacity);
    printf("stk.data: %p\n", stk->data);
    printf(" index       value\n");
    for (StackSize_t i = 0; i < stk->capacity; i++)
    {
        printf("%-5lld %-10g\n", i, (stk->data)[i]);
    }

    return 0;
}

int StackDestructor(Stack_t* stk)
{
    free(stk->data);
    stk->size = -1;
    stk->capacity = -1;
    stk->data = NULL;

    return 0;
}

