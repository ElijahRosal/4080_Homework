# Chapter 23 Challenge 1 - switch Statement

## What was implemented
A `switch` statement was added to this clox copy using the grammar from the challenge:

- `switchStmt -> "switch" "(" expression ")" "{" switchCase* defaultCase? "}"`
- `switchCase -> "case" expression ":" statement*`
- `defaultCase -> "default" ":" statement*`

Semantics implemented:

- Evaluate the switch value once.
- Compare each `case` value against that switch value using `==` semantics (`OP_EQUAL`).
- Execute only the first matching case body.
- No fallthrough.
- If no case matches, run `default` if present.

## Key implementation notes
- New scanner tokens: `TOKEN_SWITCH`, `TOKEN_CASE`, `TOKEN_DEFAULT`, `TOKEN_COLON`.
- Added `switchStatement()` in `compiler.c`.
- Added VM opcode `OP_DUP` to duplicate the switch value while testing each case.
- Each case emits an unconditional jump to the switch end after executing its body.
- The switch value is popped exactly once at statement end.

## Files changed
- `scanner.h`
- `scanner.c`
- `compiler.c`
- `chunk.h`
- `vm.c`
- `debug.c`
