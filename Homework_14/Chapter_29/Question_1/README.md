# Homework 14 - Chapter 29 - Question 1

## Question

> "If Lox was your language, how would you address this, if at all?"

## Answer

I would implement **class-qualified field names** that automatically namespace fields by the class that defines them. When a field is accessed within a method, the compiler prefixes it with the class name (e.g., `this.x` in Parent becomes `Parent.x` internally, while `this.x` in Child becomes `Child.x` internally), ensuring parent and child fields never collide. This is transparent to programmers—they write normal code, but qualification happens automatically at compile time with zero runtime overhead, making field conflicts mathematically impossible while maintaining clean semantics.


