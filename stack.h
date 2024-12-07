#define MAXSIZE SIZE_MAX - 2
#define SPOILED 0xDEADDED

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
    STACK_IS_OKAY     = 0 << 0,
    STACK_BAD_PTR     = 1 << 0,
    DATA_BAD_PTR      = 1 << 1,
    STACK_BAD_SIZE    = 1 << 2,
    STACK_BAD_CAP     = 1 << 3,
    STACK_OVERFLOW    = 1 << 4
};

Errors StackConstructor(Stack_t* stk, StackSize_t capacity);
Errors StackPush(Stack_t* stk, StackElem_t value);
Errors StackPop(Stack_t* stk, StackElem_t* value);

int StackVerify(const Stack_t* stk);
void StackDump(const Stack_t* stk, const int errnum);
int StackAssert(const Stack_t* stk);

Errors StackDestructor(Stack_t* stk);