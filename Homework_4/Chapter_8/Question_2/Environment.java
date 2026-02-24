// Elijah Rosal - CS4080 - Homework 4, Chapter 8 Question 2
// 2.23.2026
/*
Code below has been modified for Question 2 for Chapter 8 of Crafting Interpreters.

Adds explicit tracking for uninitialized variables. Accessing a variable declared
without an initializer now throws a runtime error until it is assigned.
*/
package com.craftinginterpreters.lox;
import java.util.HashMap;
import java.util.Map;
class Environment {
    static final Object UNINITIALIZED = new Object();
    final Environment enclosing;
    private final Map<String, Object> values = new HashMap<>();
    Object get(Token name) {
        if (values.containsKey(name.lexeme)) {
            Object value = values.get(name.lexeme);
            if (value == UNINITIALIZED) {
                throw new RuntimeError(name,
                        "Variable '" + name.lexeme + "' has not been initialized.");
            }
            return value;
        }
        if (enclosing != null) return enclosing.get(name);
        throw new RuntimeError(name,
                "Undefined variable '" + name.lexeme + "'.");
    }
    void assign(Token name, Object value) {
        if (values.containsKey(name.lexeme)) {
            values.put(name.lexeme, value);
            return;
        }
        if (enclosing != null) {
            enclosing.assign(name, value);
            return;
        }
        throw new RuntimeError(name,
                "Undefined variable '" + name.lexeme + "'.");
    }
    void define(String name, Object value) {
        values.put(name, value);
    }
    Environment() {
        enclosing = null;
    }
    Environment(Environment enclosing) {
        this.enclosing = enclosing;
    }
}