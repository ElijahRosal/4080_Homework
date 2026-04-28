// Elijah Rosal - CS4080 - Homework 12, Chapter 24 Question 4
// 4.28.2026
/*
Code below has been modified for Question 4 for Chapter 24 of Crafting Interpreters.

Extends native function library with 13 new utility functions: sqrt, abs, floor,
ceil, round, min, max, chr, ord, type, str, and len. These additions provide
essential mathematical, string processing, and type introspection capabilities,
transforming Lox into a practical scripting language.
*/

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <ctype.h>
#include "common.h"
#include "compiler.h"
#include "debug.h"
#include "object.h"
#include "memory.h"
#include "vm.h"
VM vm;
static Value clockNative(int argCount, Value* args) {
    return NUMBER_VAL((double)clock() / CLOCKS_PER_SEC);
}

static Value absSqrt(int argCount, Value* args) {
    if (argCount != 1 || !IS_NUMBER(args[0])) {
        runtimeError("sqrt() expects one number argument");
        return NIL_VAL;
    }
    return NUMBER_VAL(sqrt(fabs(AS_NUMBER(args[0]))));
}

static Value absNative(int argCount, Value* args) {
    if (argCount != 1 || !IS_NUMBER(args[0])) {
        runtimeError("abs() expects one number argument");
        return NIL_VAL;
    }
    return NUMBER_VAL(fabs(AS_NUMBER(args[0])));
}

static Value floorNative(int argCount, Value* args) {
    if (argCount != 1 || !IS_NUMBER(args[0])) {
        runtimeError("floor() expects one number argument");
        return NIL_VAL;
    }
    return NUMBER_VAL(floor(AS_NUMBER(args[0])));
}

static Value ceilNative(int argCount, Value* args) {
    if (argCount != 1 || !IS_NUMBER(args[0])) {
        runtimeError("ceil() expects one number argument");
        return NIL_VAL;
    }
    return NUMBER_VAL(ceil(AS_NUMBER(args[0])));
}

static Value roundNative(int argCount, Value* args) {
    if (argCount != 1 || !IS_NUMBER(args[0])) {
        runtimeError("round() expects one number argument");
        return NIL_VAL;
    }
    return NUMBER_VAL(round(AS_NUMBER(args[0])));
}

static Value minNative(int argCount, Value* args) {
    if (argCount < 1 || !IS_NUMBER(args[0])) {
        runtimeError("min() expects at least one number argument");
        return NIL_VAL;
    }
    double min = AS_NUMBER(args[0]);
    for (int i = 1; i < argCount; i++) {
        if (!IS_NUMBER(args[i])) {
            runtimeError("min() expects all arguments to be numbers");
            return NIL_VAL;
        }
        double val = AS_NUMBER(args[i]);
        if (val < min) min = val;
    }
    return NUMBER_VAL(min);
}

static Value maxNative(int argCount, Value* args) {
    if (argCount < 1 || !IS_NUMBER(args[0])) {
        runtimeError("max() expects at least one number argument");
        return NIL_VAL;
    }
    double max = AS_NUMBER(args[0]);
    for (int i = 1; i < argCount; i++) {
        if (!IS_NUMBER(args[i])) {
            runtimeError("max() expects all arguments to be numbers");
            return NIL_VAL;
        }
        double val = AS_NUMBER(args[i]);
        if (val > max) max = val;
    }
    return NUMBER_VAL(max);
}

static Value chrNative(int argCount, Value* args) {
    if (argCount != 1 || !IS_NUMBER(args[0])) {
        runtimeError("chr() expects one number argument");
        return NIL_VAL;
    }
    int code = (int)AS_NUMBER(args[0]);
    if (code < 0 || code > 127) {
        runtimeError("chr() argument must be between 0 and 127");
        return NIL_VAL;
    }
    char str[2] = { (char)code, '\0' };
    return OBJ_VAL(copyString(str, 1));
}

static Value ordNative(int argCount, Value* args) {
    if (argCount != 1 || !IS_STRING(args[0])) {
        runtimeError("ord() expects one string argument");
        return NIL_VAL;
    }
    ObjString* str = AS_STRING(args[0]);
    if (str->length != 1) {
        runtimeError("ord() expects a string of length 1");
        return NIL_VAL;
    }
    return NUMBER_VAL((double)(unsigned char)str->chars[0]);
}

static Value typeNative(int argCount, Value* args) {
    if (argCount != 1) {
        runtimeError("type() expects exactly one argument");
        return NIL_VAL;
    }
    
    Value val = args[0];
    const char* typeName;
    
    if (IS_BOOL(val)) typeName = "bool";
    else if (IS_NIL(val)) typeName = "nil";
    else if (IS_NUMBER(val)) typeName = "number";
    else if (IS_STRING(val)) typeName = "string";
    else if (IS_FUNCTION(val)) typeName = "function";
    else if (IS_NATIVE(val)) typeName = "native";
    else typeName = "object";
    
    return OBJ_VAL(copyString(typeName, (int)strlen(typeName)));
}

static Value strNative(int argCount, Value* args) {
    if (argCount != 1) {
        runtimeError("str() expects exactly one argument");
        return NIL_VAL;
    }
    
    Value val = args[0];
    if (IS_STRING(val)) {
        return val;
    } else if (IS_NUMBER(val)) {
        double num = AS_NUMBER(val);
        char buffer[32];
        if (num == (long long)num) {
            sprintf(buffer, "%lld", (long long)num);
        } else {
            sprintf(buffer, "%.15g", num);
        }
        return OBJ_VAL(copyString(buffer, (int)strlen(buffer)));
    } else if (IS_BOOL(val)) {
        return OBJ_VAL(copyString(AS_BOOL(val) ? "true" : "false", 
            AS_BOOL(val) ? 4 : 5));
    } else if (IS_NIL(val)) {
        return OBJ_VAL(copyString("nil", 3));
    }
    
    runtimeError("Cannot convert value to string");
    return NIL_VAL;
}

static Value lenNative(int argCount, Value* args) {
    if (argCount != 1 || !IS_STRING(args[0])) {
        runtimeError("len() expects one string argument");
        return NIL_VAL;
    }
    return NUMBER_VAL((double)AS_STRING(args[0])->length);
}

static void resetStack() {
    vm.stackTop = vm.stack;
    vm.frameCount = 0;
}
static void runtimeError(const char* format, ...) {
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
    fputs("\n", stderr);
    for (int i = vm.frameCount - 1; i >= 0; i--) {
        CallFrame* frame = &vm.frames[i];
        ObjFunction* function = frame->function;
        size_t instruction = frame->ip - function->chunk.code - 1;
        fprintf(stderr, "[line %d] in ",
        function->chunk.lines[instruction]);
        if (function->name == NULL) {
            fprintf(stderr, "script\n");
        } else {
            fprintf(stderr, "%s()\n", function->name->chars);
        }
    }
    resetStack();
}
static void defineNative(const char* name, NativeFn function)
{
    push(OBJ_VAL(copyString(name, (int)strlen(name))));
    push(OBJ_VAL(newNative(function)));
    tableSet(&vm.globals, AS_STRING(vm.stack[0]), vm.stack[1]);
    pop();
    pop();
}
void initVM() {
    resetStack();
    vm.objects = NULL;
    initTable(&vm.globals);
    initTable(&vm.strings);
    defineNative("clock", clockNative);
    defineNative("sqrt", absSqrt);
    defineNative("abs", absNative);
    defineNative("floor", floorNative);
    defineNative("ceil", ceilNative);
    defineNative("round", roundNative);
    defineNative("min", minNative);
    defineNative("max", maxNative);
    defineNative("chr", chrNative);
    defineNative("ord", ordNative);
    defineNative("type", typeNative);
    defineNative("str", strNative);
    defineNative("len", lenNative);
}
void freeVM() {
    freeTable(&vm.globals);
    freeTable(&vm.strings);
    freeObjects();
}
void push(Value value) {
    *vm.stackTop = value;
    vm.stackTop++;
}
Value pop() {
    vm.stackTop--;
    return *vm.stackTop;
}
static Value peek(int distance) {
    return vm.stackTop[-1 - distance];
}
static bool call(ObjFunction* function, int argCount) {
    if (argCount != function->arity) {
        runtimeError("Expected %d arguments but got %d.",
        function->arity, argCount);
        return false;
    }
    if (vm.frameCount == FRAMES_MAX) {
        runtimeError("Stack overflow.");
        return false;
    }
    CallFrame* frame = &vm.frames[vm.frameCount++];
    frame->function = function;
    frame->ip = function->chunk.code;
    frame->slots = vm.stackTop - argCount - 1;
    return true;
}
static bool callValue(Value callee, int argCount) {
    if (IS_OBJ(callee)) {
        switch (OBJ_TYPE(callee)) {
            case OBJ_FUNCTION:
                return call(AS_FUNCTION(callee), argCount);
            case OBJ_NATIVE: {
                NativeFn native = AS_NATIVE(callee);
                Value result = native(argCount, vm.stackTop -
               argCount);
                vm.stackTop -= argCount + 1;
                push(result);
                return true;
            }
            default:
                break; // Non-callable object type.
        }
    }
    runtimeError("Can only call functions and classes.");
    return false;
}
static bool isFalsey(Value value) {
    return IS_NIL(value) || (IS_BOOL(value) && !AS_BOOL(value));
}
static void concatenate() {
    ObjString* b = AS_STRING(pop());
    ObjString* a = AS_STRING(pop());
    int length = a->length + b->length;
    char* chars = ALLOCATE(char, length + 1);
    memcpy(chars, a->chars, a->length);
    memcpy(chars + a->length, b->chars, b->length);
    chars[length] = '\0';
    ObjString* result = takeString(chars, length);
    push(OBJ_VAL(result));
}
static InterpretResult run() {
    CallFrame* frame = &vm.frames[vm.frameCount - 1];
#define READ_BYTE() (*frame->ip++)
#define READ_SHORT() \
(frame->ip += 2, \
(uint16_t)((frame->ip[-2] << 8) | frame->ip[-1]))
#define READ_CONSTANT() \
(frame->function->chunk.constants.values[READ_BYTE()])
#define READ_STRING() AS_STRING(READ_CONSTANT())
#define BINARY_OP(valueType, op) \
do { \
if (!IS_NUMBER(peek(0)) || !IS_NUMBER(peek(1))) { \
runtimeError("Operands must be numbers."); \
return INTERPRET_RUNTIME_ERROR; \
} \
double b = AS_NUMBER(pop()); \
double a = AS_NUMBER(pop()); \
push(valueType(a op b)); \
} while (false)
    for (;;) {
        #ifdef DEBUG_TRACE_EXECUTION
        printf(" ");
        for (Value* slot = vm.stack; slot < vm.stackTop; slot++) {
            printf("[ ");
            printValue(*slot);
            printf(" ]");
        }
        printf("\n");
        disassembleInstruction(&frame->function->chunk,
         (int)(frame->ip - frame->function->chunk.code));
        #endif
        uint8_t instruction;
        switch (instruction = READ_BYTE()) {
            case OP_CONSTANT: {
                Value constant = READ_CONSTANT();
                push(constant);
                break;
            }
            case OP_NIL: push(NIL_VAL); break;
            case OP_TRUE: push(BOOL_VAL(true)); break;
            case OP_FALSE: push(BOOL_VAL(false)); break;
            case OP_POP: pop(); break;
            case OP_GET_LOCAL: {
                uint8_t slot = READ_BYTE();
                push(frame->slots[slot]);
                break;
            }
            case OP_SET_LOCAL: {
                uint8_t slot = READ_BYTE();
                frame->slots[slot] = peek(0);
                break;
            }
            case OP_GET_GLOBAL: {
                ObjString* name = READ_STRING();
                Value value;
                if (!tableGet(&vm.globals, name, &value)) {
                    runtimeError("Undefined variable '%s'.", name->chars);
                    return INTERPRET_RUNTIME_ERROR;
                }
                push(value);
                break;
            }
            case OP_DEFINE_GLOBAL: {
                ObjString* name = READ_STRING();
                tableSet(&vm.globals, name, peek(0));
                pop();
                break;
            }
            case OP_SET_GLOBAL: {
                ObjString* name = READ_STRING();
                if (tableSet(&vm.globals, name, peek(0))) {
                    tableDelete(&vm.globals, name);
                    runtimeError("Undefined variable '%s'.", name->chars);
                    return INTERPRET_RUNTIME_ERROR;
                }
                break;
            }
            case OP_EQUAL: {
                Value b = pop();
                Value a = pop();
                push(BOOL_VAL(valuesEqual(a, b)));
                break;
            }
            case OP_GREATER: BINARY_OP(BOOL_VAL, >); break;
            case OP_LESS: BINARY_OP(BOOL_VAL, <); break;
            case OP_ADD: {
                if (IS_STRING(peek(0)) && IS_STRING(peek(1))) {
                    concatenate();
                } else if (IS_NUMBER(peek(0)) && IS_NUMBER(peek(1))) {
                    double b = AS_NUMBER(pop());
                    double a = AS_NUMBER(pop());
                    push(NUMBER_VAL(a + b));
                } else {
                    runtimeError(
                    "Operands must be two numbers or two strings.");
                    return INTERPRET_RUNTIME_ERROR;
                }
                break;
            }

            case OP_SUBTRACT: BINARY_OP(NUMBER_VAL, -); break;
            case OP_MULTIPLY: BINARY_OP(NUMBER_VAL, *); break;
            case OP_DIVIDE: BINARY_OP(NUMBER_VAL, /); break;
            case OP_NOT:
                push(BOOL_VAL(isFalsey(pop())));
                break;
            case OP_NEGATE:
                if (!IS_NUMBER(peek(0))) {
                    runtimeError("Operand must be a number.");
                    return INTERPRET_RUNTIME_ERROR;
                }
                push(NUMBER_VAL(-AS_NUMBER(pop())));
                break;
            case OP_PRINT: {
                printValue(pop());
                printf("\n");
                break;
            }
            case OP_JUMP: {
                uint16_t offset = READ_SHORT();
                frame->ip += offset;
                break;
            }
            case OP_JUMP_IF_FALSE: {
                uint16_t offset = READ_SHORT();
                if (isFalsey(peek(0))) frame->ip += offset;
                break;
            }
            case OP_LOOP: {
                uint16_t offset = READ_SHORT();
                frame->ip -= offset;
                break;
            }
            case OP_CALL: {
                int argCount = READ_BYTE();
                if (!callValue(peek(argCount), argCount)) {
                    return INTERPRET_RUNTIME_ERROR;
                }
                frame = &vm.frames[vm.frameCount - 1];
                break;
            }
            case OP_RETURN: {
                Value result = pop();
                vm.frameCount--;
                if (vm.frameCount == 0) {
                    pop();
                    return INTERPRET_OK;
                }
                vm.stackTop = frame->slots;
                push(result);
                frame = &vm.frames[vm.frameCount - 1];
                break;
            }
        }
    }
#undef READ_BYTE
#undef READ_SHORT
#undef READ_CONSTANT
#undef READ_STRING
#undef BINARY_OP
}
InterpretResult interpret(const char* source) {
    ObjFunction* function = compile(source);
    if (function == NULL) return INTERPRET_COMPILE_ERROR;
    push(OBJ_VAL(function));
    call(function, 0);
    return run();

}