# Question 2: Arity Checking for Native Functions

## Overview

Previously, native functions didn't validate the number of arguments passed to them. This could lead to undefined behavior if a user called a native function with incorrect argument counts, potentially causing the function to read uninitialized memory or access invalid data.

This implementation adds proper arity checking to native functions, ensuring that runtime errors are raised when incorrect argument counts are used.

## Changes Made

### 1. Modified object.h
- Added an `arity` field to the `ObjNative` struct to store the expected number of arguments

```c
typedef struct {
    Obj obj;
    NativeFn function;
    int arity;  // NEW: Expected number of arguments
} ObjNative;
```

### 2. Modified object.c
- Updated `newNative()` function to accept and store an arity parameter:

```c
ObjNative* newNative(NativeFn function, int arity) {
    ObjNative* native = ALLOCATE_OBJ(ObjNative, OBJ_NATIVE);
    native->function = function;
    native->arity = arity;
    return native;
}
```

### 3. Modified vm.c
- Updated `defineNative()` to pass arity when creating native functions:
  ```c
  static void defineNative(const char* name, NativeFn function, int arity)
  ```

- Updated `initVM()` to specify arity for native functions:
  ```c
  defineNative("clock", clockNative, 0);  // clock takes 0 arguments
  ```

- Enhanced `callValue()` to validate argument count for native functions:
  ```c
  case OBJ_NATIVE: {
      ObjNative* native = (ObjNative*)AS_OBJ(callee);
      if (argCount != native->arity) {
          runtimeError("Expected %d arguments but got %d.",
              native->arity, argCount);
          return false;
      }
      // ... call native function ...
  }
  ```

## Benefits

1. **Memory Safety**: Prevents undefined behavior from reading uninitialized memory
2. **Runtime Errors**: Users get clear error messages when calling native functions incorrectly
3. **Type Safety**: Similar to how Lox user-defined functions already validate arguments
4. **Consistency**: Native functions now have the same argument validation as user functions

## Example Usage

### Correct Usage
```lox
print clock();  // Valid - clock takes 0 arguments
```

### Incorrect Usage (now caught)
```lox
print clock(1);  // Runtime error: Expected 0 arguments but got 1.
```

## Performance Impact

Negligible - the arity check is just one integer comparison that happens only during native function calls, which are relatively infrequent operations compared to user-defined function calls.

## Testing

Run the test file:
```bash
./cmake-build-debug/lox test_arity.lox
```

This demonstrates that:
- Native functions with correct arguments work properly
- The arity validation system is in place and ready for additional native functions
