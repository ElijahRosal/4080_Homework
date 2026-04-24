# Chapter 22 Challenge 2

## Question
How should code like `var a = a;` be handled?

## Answer
Different languages choose different semantics:
- Some dynamic languages may treat it as reading the current binding (if one exists).
- Some languages reject it as using a variable before initialization.
- Others allow shadowing and define precise initialization order semantics.

For Lox/clox, rejecting it is a good choice because:
- It prevents subtle bugs from accidental self-reference.
- It matches the existing Chapter 22 rule: local variables are declared before initializer but are not readable until initialized.
- It gives a clear, early compile-time error (`Can't read local variable in its own initializer.`).

No code changes were needed beyond the Chapter 22 baseline behavior already present in this project copy.
