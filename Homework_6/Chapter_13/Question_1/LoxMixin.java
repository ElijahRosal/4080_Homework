// Elijah Rosal - CS4080 - Homework 6, Chapter 13 Question 1
// 3.12.2026
/*
Code below has been modified for Question 1 for Chapter 13 of Crafting Interpreters.

Defines a mixin as a named bundle of methods that classes can include via the
"with" clause. Mixins are not callable and cannot be instantiated directly.
*/

package com.craftinginterpreters.lox;

import java.util.Map;

class LoxMixin {
    final String name;
    private final Map<String, LoxFunction> methods;

    LoxMixin(String name, Map<String, LoxFunction> methods) {
        this.name = name;
        this.methods = methods;
    }

    LoxFunction findMethod(String name) {
        return methods.getOrDefault(name, null);
    }

    @Override
    public String toString() {
        return "<mixin " + name + ">";
    }
}
