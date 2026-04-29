# Homework 12, Chapter 25, Question 2

Read the design note about closing over the loop variable and decide how Lox should behave.

## Answer

Lox should create a new variable for each loop iteration.

That is the behavior users usually expect when they write a loop that introduces a variable in the header. If a closure captures the loop variable, it should capture the value for that specific iteration, not a single mutable variable shared by the whole loop. Otherwise, the program is technically consistent but very surprising, especially when the closure is used later after the loop has advanced.

This matches the pragmatic behavior of modern languages like JavaScript with `let` and many foreach-style loops in other languages. It is also easier to explain than the alternative: each iteration gets its own fresh binding, so closures naturally preserve the value they saw when they were created.

For old-style C-like `for` loops, the increment clause still looks like mutation, but the useful behavior is the same. The loop variable should be copied into a fresh iteration-local binding before the body runs. That keeps normal loop behavior intact while making captured values predictable.

In short, I would choose the per-iteration variable model because it is more intuitive, more useful with closures, and less likely to surprise users.
