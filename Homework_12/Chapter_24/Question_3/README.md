# Question 3: Native Function Error Handling

## Overview

In the original implementation, native functions had no way to signal runtime errors. This was problematic because:

1. Native functions could not report type mismatches (e.g., passing a string to a numeric function)
2. Invalid operations had no error reporting mechanism
3. There was no way to distinguish between a valid `nil` result and an error condition

This implementation extends the native function system to support runtime error signaling, allowing native functions to communicate errors back to the Lox runtime.

## Architecture

### VM State Extension

Added error state to the VM structure:

```c
typedef struct {
    // ... existing fields ...
    bool hasNativeError;
    char nativeError[NATIVE_ERROR_MAX];
} VM;
```

### Error Signaling Functions

Two new functions in `vm.h`/`vm.c`:

```c
// Called by native functions to signal an error
void setNativeError(const char* format, ...);

// Checked after native function returns
bool getNativeError();
```

## Implementation Details

### 1. Error State Management

When a native function detects an error:
```c
setNativeError("sqrt() argument must be a number");
return NIL_VAL;  // Return dummy value
```

### 2. Error Checking in callValue()

After calling a native function:
```c
case OBJ_NATIVE: {
    NativeFn native = AS_NATIVE(callee);
    Value result = native(argCount, vm.stackTop - argCount);
    
    // Check if native function signaled an error
    if (getNativeError()) {
        runtimeError("%s", vm.nativeError);
        vm.hasNativeError = false;
        return false;
    }
    // ... continue normally ...
}
```

### 3. Example Native Function: sqrt()

The `sqrt()` function demonstrates error handling:

```c
static Value sqrtNative(int argCount, Value* args) {
    if (!IS_NUMBER(args[0])) {
        setNativeError("sqrt() argument must be a number");
        return NIL_VAL;
    }
    
    double value = AS_NUMBER(args[0]);
    if (value < 0) {
        setNativeError("sqrt() argument cannot be negative: %g", value);
        return NIL_VAL;
    }
    
    return NUMBER_VAL(sqrt(value));
}
```

## Usage Examples

### Valid Usage
```lox
print sqrt(16);  // Output: 4
```

### Error Cases Caught
```lox
sqrt(-1);          // Runtime error: sqrt() argument cannot be negative
sqrt("hello");     // Runtime error: sqrt() argument must be a number
```

## Performance Impact

**Negligible** - The error check is a simple boolean test that only happens during native function calls. Performance is virtually identical to the previous implementation for the happy path.

### Performance Measurement

Test with `test_error_demo.lox` and normal programs:
- Programs without native function calls: No change
- Programs with native function calls: Minimal overhead (one boolean check per call)
- Error conditions: Same performance as calling `runtimeError()`

## Benefits

1. **Type Safety**: Native functions can validate argument types before processing
2. **Rich Error Messages**: Functions can provide detailed error information
3. **Memory Safety**: Prevents undefined behavior from invalid operations
4. **User Experience**: Clear, actionable error messages for users
5. **Extensibility**: Easy to add more native functions with error handling

## Adding New Native Functions with Error Handling

To add a new native function with error handling:

1. Define the function:
```c
static Value myFunction(int argCount, Value* args) {
    if (/* invalid condition */) {
        setNativeError("Error message here");
        return NIL_VAL;
    }
    return /* result */;
}
```

2. Register it in `initVM()`:
```c
defineNative("myFunc", myFunction);
```

## Testing

Run the test files:

```bash
# Test valid sqrt calls
./cmake-build-debug/lox test_errors.lox

# Demonstrate error handling (this will trigger an error)
./cmake-build-debug/lox test_error_demo.lox
```

## Trade-offs

**Advantages:**
- Enables proper error handling for native functions
- Simple, non-invasive implementation
- No performance penalty in the happy path
- Follows the same error reporting pattern as user functions

**Disadvantages:**
- Native function errors are converted to runtime errors (though this is the desired behavior)
- Requires discipline from C developers implementing native functions
- Error messages are limited to NATIVE_ERROR_MAX characters (currently 512)

## Comparison with Previous Systems

| Feature | Before | After |
|---------|--------|-------|
| Type checking | None | Yes |
| Error messages | None | Full support |
| Type validation | None | Full support |
| Memory safety | Unsafe | Safe |
