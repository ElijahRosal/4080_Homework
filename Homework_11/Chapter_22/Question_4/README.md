# Chapter 22 Challenge 4

## Goal
Allow more than 256 locals in scope.

## What was changed
- `common.h`: added `UINT16_COUNT`.
- `chunk.h`: added long-form local opcodes:
  - `OP_GET_LOCAL_LONG`
  - `OP_SET_LOCAL_LONG`
- `compiler.c`:
  - Local table enlarged to `UINT16_COUNT`.
  - Added 16-bit operand emission helper.
  - Local variable bytecode emission now uses:
    - 8-bit local opcodes for slots <= 255
    - 16-bit local opcodes for larger slots
  - Compiler instance in `compile()` is static to avoid large stack allocation.
- `vm.c`: added `READ_SHORT()` and execution for long local opcodes.
- `debug.c`: disassembler now prints long local opcodes with 16-bit slots.
- `vm.h`: raised `STACK_MAX` to `UINT16_COUNT` so runtime stack supports additional locals.

This preserves existing behavior while extending local slot addressing beyond one byte.
