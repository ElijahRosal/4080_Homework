# Question 1

Many dynamically typed languages treat a missing field as a non-fatal lookup. JavaScript returns `undefined`, Ruby commonly returns `nil` from permissive access patterns, and Smalltalk-style systems often route missing messages through a fallback hook instead of crashing immediately. I chose the simplest Lox-friendly behavior: reading a missing field now returns `nil` instead of aborting the VM.

That keeps field reads predictable and lets user code continue running. It also matches Lox's existing use of `nil` as the language's general "absence" value.
