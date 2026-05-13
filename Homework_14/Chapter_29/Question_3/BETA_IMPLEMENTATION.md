# BETA-Style Method Overriding in clox

## Overview

This implementation replaces Lox's standard object-oriented method resolution with BETA-style method overriding. In BETA:

1. **Method lookup is top-down**: When calling a method on an instance, the method highest in the class hierarchy takes precedence.
2. **Inner keyword**: Used to call the next method down the inheritance chain, giving the superclass method control over when/if the subclass can participate.
3. **Superclass controls override**: The superclass method must explicitly call `inner()` to allow subclass refinement.

## Key Changes from Standard Lox

### Data Structures

1. **ObjClass** - Now stores a superclass pointer:
   ```c
   typedef struct ObjClass {
       Obj obj;
       ObjString* name;
       struct ObjClass* superclass;  // Direct superclass reference
       Table methods;
   } ObjClass;
   ```

2. **ObjBoundMethod** - Now tracks which class defined the method:
   ```c
   typedef struct {
       Obj obj;
       Value receiver;
       ObjClosure* method;
       ObjClass* definingClass;  // Class where this method is defined
   } ObjBoundMethod;
   ```

3. **CallFrame** - Tracks the defining class of the current method:
   ```c
   typedef struct {
       ObjClosure* closure;
       uint8_t* ip;
       Value* slots;
       ObjClass* definingClass;  // For inner() support
   } CallFrame;
   ```

### Compiler Changes

1. **New TOKEN_INNER keyword** in scanner
2. **Method name tracking** in Compiler struct for use with `inner()`
3. **inner_() function** that generates OP_INNER_INVOKE bytecode
4. **OP_INNER_INVOKE opcode** for calling the next method down the hierarchy

### VM Changes

1. **Top-down method lookup**:
   - `findMethodDefiningClass()` - Recursively searches from superclass down
   - First checks superclass chain, then the current class
   - Returns both the method and the class that defined it

2. **OP_INHERIT behavior**:
   - No longer copies methods down
   - Simply stores the superclass pointer
   - Methods are looked up dynamically

3. **OP_INNER_INVOKE execution**:
   - Looks for a method with the same name in the subclass
   - Only searches in the direct instance's class, not further down
   - Does nothing if no inner method exists

## Example Usage

```lox
class Doughnut {
  cook() {
    print "Fry until golden brown.";
    inner();  // Call any subclass override
    print "Place in a nice box.";
  }
}

class BostonCream < Doughnut {
  cook() {
    print "Pipe full of custard and coat with chocolate.";
  }
}

BostonCream().cook();
```

Output:
```
Fry until golden brown.
Pipe full of custard and coat with chocolate.
Place in a nice box.
```

## Implementation Details

### Method Resolution Order

1. When `instance.method()` is called:
   - Find the topmost superclass
   - Recursively search from superclass down to instance's class
   - Use the first matching method found

2. When `inner()` is called within a method:
   - Get the current method's defining class from the call frame
   - Look for a method with the same name in the instance's class
   - Call it if found; do nothing if not found

### Performance Considerations

- **No method caching**: Every method call performs a top-down search
- **Recursive lookup**: Uses recursion up the superclass chain
- **Single inheritance only**: Matches Lox's linear hierarchy model
- **Stack efficiency**: Method defining class stored in call frame

## Limitations

1. Methods are not cached - every lookup starts from the top
2. Circular inheritance detection is NOT implemented
3. Multiple inheritance is not supported (inherited from Lox)
4. `super` keyword is NOT supported (replaced by BETA's `inner`)
5. `inner()` has no effect if there's no method in the subclass

## Testing

See `test_inner.lox` for the example from the challenge. Expected output:
```
Fry until golden brown.
Pipe full of custard and coat with chocolate.
Place in a nice box.
```

## Efficiency Notes

This implementation prioritizes correctness over performance. Some optimization opportunities:

1. Cache method lookups per (class, methodName) pair
2. Use a method resolution order (MRO) list computed at inheritance setup
3. Add a "next method" pointer to each method when inherited
4. Use inline caching at call sites

For the purposes of this challenge, the current implementation demonstrates the semantic changes required for BETA-style method resolution while maintaining reasonable clarity.
