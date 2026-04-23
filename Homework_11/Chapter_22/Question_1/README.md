# Chapter 22 Challenge 1

## Goal
Replace linear local-variable lookup with a more efficient strategy.

## What was changed
- Updated `compiler.c` to use a hash-bucket chain for locals in scope.
- Added per-local metadata (`hash`, `next`) to support bucket-linked lookup.
- Added compiler bucket table (`localBuckets`) and initialization in `initCompiler()`.
- `resolveLocal()` now hashes identifier lexemes and searches only one chain.
- `declareVariable()` duplicate checks now walk only matching-name hash chains.
- `endScope()` restores bucket heads while popping locals to preserve shadowing behavior.

## Complexity tradeoff
- Average lookup improves from O(n) to near O(1) with short collision chains.
- Additional complexity is moderate but contained to compiler bookkeeping.
- For tiny scripts this likely does not matter much, but for larger scopes it is a useful optimization.
