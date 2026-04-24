# Chapter 23 Challenge 2 - continue Statement

## What was implemented
A `continue;` statement was added with grammar:

- `continueStmt -> "continue" ";"`

Semantics implemented:

- `continue` jumps to the nearest enclosing loop's continue target.
- In `while`, it jumps to the top of the loop condition.
- In `for`, it jumps to the increment clause when present; otherwise to the loop condition.
- Compile-time error when `continue` appears outside any loop.

## Scope handling
Before emitting the backward jump, the compiler emits `OP_POP` for locals declared in scopes nested inside the loop body that would be exited by `continue`.

This preserves stack discipline and ensures locals are discarded exactly as if control had reached the end of those scopes normally.

## Key implementation notes
- New scanner token: `TOKEN_CONTINUE`.
- Added `continueStatement()` in `compiler.c`.
- Added loop context tracking via an internal `Loop` struct (`continueTarget`, `scopeDepth`, `enclosing`) to resolve nearest loop and cleanup behavior.
- `whileStatement()` and `forStatement()` now install/remove loop context while compiling their bodies.

## Files changed
- `scanner.h`
- `scanner.c`
- `compiler.c`
