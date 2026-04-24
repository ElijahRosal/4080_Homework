# Chapter 23 Challenge 3 - Novel Control Flow Feature

## Proposed feature: `repeat ... until (...)`

### Syntax

- `repeatStmt -> "repeat" statement "until" "(" expression ")" ";"`

Example:

```lox
repeat {
  print i;
  i = i + 1;
} until (i >= 10);
```

### Semantics

- Executes the body at least once (post-test loop).
- After each iteration, evaluate the `until` condition.
- If the condition is truthy, exit the loop.
- If falsey, iterate again.

### Why this is useful

It expresses an intentional "run once, then test" loop directly, avoiding common patterns like:

```lox
while (true) {
  ...
  if (done) break;
}
```

This improves readability in input-processing and retry workflows where one pass is mandatory.

### Bytecode compilation sketch in clox style

1. Record `loopStart` at the beginning of the body.
2. Compile the body statement.
3. Compile `until (condition)` expression.
4. Emit `OP_JUMP_IF_FALSE` back-edge logic (or equivalent combination):
   - if condition is falsey, jump back to `loopStart`.
5. Pop condition value on both control paths to preserve zero net stack effect.

### Notes

This folder contains the full copied C source baseline. The feature here is a design proposal (not implemented), as the challenge asks to invent and evaluate a novel control-flow construct.
