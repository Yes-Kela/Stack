#ifndef STACK_H
#define STACK_H

#define MAXSIZE SIZE_MAX - 2
#define SPOILED 0xBA51115A
#define LEFT_CANARY 0xBAD1ABA
#define RIGHT_CANARY 0xDEADDED
#define STACK_ASSERT_START(stk) StackAssert(stk, __FILE__, __func__, __LINE__, "beginning")
#define STACK_ASSERT_END(stk) StackAssert(stk, __FILE__, __func__, __LINE__, "ending")

typedef unsigned int StackElem_t;
typedef unsigned long int StackCanary_t;
typedef size_t StackSize_t;
typedef struct
{
    char* data;
    StackSize_t size;
    StackSize_t capacity;
} Stack_t;

enum Errors
{
    STACK_IS_OKAY         = 0 << 0,
    STACK_CTOR_SUCCESS    = 0 << 0,
    STACK_PUSH_SUCCESS    = 0 << 0,
    STACK_POP_SUCCESS     = 0 << 0,
    STACK_DTOR_SUCCESS    = 0 << 0,

    STACK_BAD_PTR         = 1 << 0,
    DATA_BAD_PTR          = 1 << 1,
    STACK_BAD_SIZE        = 1 << 2,
    STACK_BAD_CAP         = 1 << 3,
    STACK_OVERFLOW        = 1 << 4,
    MEM_ALLOC_ERROR       = 1 << 5,
    MEM_REALLOC_ERROR     = 1 << 6,
    EMPTY_STACK           = 1 << 7,
    LEFT_CANARY_DIED      = 1 << 8,
    RIGHT_CANARY_DIED     = 1 << 9,
    ATTACK_FROM_THE_LEFT  = 1 << 10,
    ATTACK_FROM_THE_RIGHT = 1 << 11,

    STACK_CTOR_FAILED     = 1 << 12,
    STACK_PUSH_FAILED     = 1 << 13,
    STACK_POP_FAILED      = 1 << 14,
    STACK_DTOR_FAILED     = 1 << 15
};

Errors StackConstructor(Stack_t* stk, StackSize_t capacity);
Errors StackPush(Stack_t* stk, StackElem_t value);
Errors StackPop(Stack_t* stk, StackElem_t* value);
Errors StackDestructor(Stack_t* stk);
StackSize_t StackRealSize(const Stack_t* stk);
StackSize_t StackRealCapacity(const Stack_t* stk);

int StackVerify(const Stack_t* stk);
void StackDump(const Stack_t* stk, const int errnum, const char* file_name, const char* func_name, const int line, const char* mode);
int StackAssert(const Stack_t* stk, const char* file_name, const char* func_name, const int line, const char* mode);
void StackStop(Stack_t* stk, FILE* file);

#endif /* STACK_H */
