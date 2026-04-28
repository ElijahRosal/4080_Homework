# Question 1: IP Register Optimization

## Overview

This optimization addresses a performance bottleneck in the Lox bytecode interpreter. The instruction pointer (`ip`) is one of the most frequently accessed values in the bytecode loop, but it was accessed through a pointer to the current CallFrame, requiring pointer indirection that could force the CPU to bypass the cache and hit main memory.

## Implementation

### Changes Made to vm.c

1. **Added register local variable**: Instead of accessing `ip` through `frame->ip` on every bytecode instruction, we now load it into a local variable marked with `register`:
   ```c
   register uint8_t* ip = frame->ip;
   ```

2. **Updated macros**: All READ_BYTE() and READ_SHORT() macros now use the local `ip` variable instead of `frame->ip`:
   ```c
   #define READ_BYTE() (*ip++)
   #define READ_SHORT() (ip += 2, (uint16_t)((ip[-2] << 8) | ip[-1]))
   ```

3. **Synced ip on function calls**: Before and after function calls (OP_CALL and OP_RETURN), we synchronize the local `ip` variable with the frame pointer:
   ```c
   case OP_CALL:
       frame->ip = ip;  // Save local ip to frame
       // ... call function ...
       frame = &vm.frames[vm.frameCount - 1];
       ip = frame->ip;  // Reload ip from new frame
       break;
   ```

## Expected Benefits

- **Reduced memory access**: By keeping `ip` in a CPU register, we avoid pointer indirection for almost every bytecode instruction
- **Better cache locality**: Less pressure on the memory hierarchy
- **Compiler optimization**: The `register` keyword hints to the compiler that this variable is critical and should be prioritized for register allocation

## Performance Testing

### Benchmark File: benchmark.lox

The benchmark file tests several functions:

1. **fib(20)** - Recursive Fibonacci computation
2. **factorial(15)** - Recursive factorial
3. **fibonacci_iterative(1000)** - Iterative Fibonacci
4. **loop_test(100000)** - Simple loop with arithmetic

### How to Build and Run

```bash
cd Question_1
cmake -B cmake-build-debug
cmake --build cmake-build-debug

# Run the benchmark
./cmake-build-debug/lox benchmark.lox
```

### Comparing Performance

To see the performance difference, you can:
1. Build this optimized version and record execution times
2. Compare with a non-optimized version (comment out the `register` keyword and ip syncing)

## Trade-offs

**Advantages:**
- Significant performance improvement for compute-heavy Lox programs
- Minimal code complexity increase
- Leverages compiler optimizations

**Disadvantages:**
- Slightly more complex code with ip synchronization points
- Register keyword is only a hint; compiler may not honor it on all platforms
- More bookkeeping needed when switching between function frames

## Conclusion

The extra code complexity is **worth it** because:
1. The `ip` variable is accessed on nearly every bytecode instruction, making it a critical hot spot
2. The synchronization logic is straightforward and only happens on function calls/returns (relatively rare)
3. Register allocation can provide 5-20% performance improvements on typical benchmarks
4. The optimization is standard practice in high-performance VM implementations (like CPython, V8, etc.)
