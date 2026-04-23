# Chapter 22 Challenge 3

## Keyword choice
Used `const` for single-assignment variables.

## Why `const`
- Familiar from JavaScript/C/C++ ecosystems.
- Clearly communicates immutability intent.
- Easy to read alongside existing `var` declarations.

## What was changed
- `scanner.h`: added `TOKEN_CONST`.
- `scanner.c`: recognizes `const` in identifier keyword matching.
- `compiler.c`:
  - Local declarations now track mutability.
  - Added global declaration mutability tracking for this compiled chunk.
  - Parser accepts `const` declarations.
  - Assignment to a `const` variable reports compile-time error: `Can't assign to a const variable.`

Both local and known-global declarations in the same compile pass are checked.
