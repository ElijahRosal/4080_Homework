package com.craftinginterpreters.lox;
// Elijah Rosal - CS4080 - Homework 5, Chapter 11 Question 4
// 3.5.2026
/*
Code below has been modified for Question 4 for Chapter 11 of Crafting Interpreters.

Updates the environment to support indexed local-variable slots so resolved locals
can be accessed by (scope distance, slot index) instead of name-map lookup.
*/

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
class Environment {
    static final Object UNINITIALIZED = new Object();
    final Environment enclosing;
    private final Map<String, Object> values = new HashMap<>();
    // Slot array for fast local access by resolver-assigned index.
    private final List<Object> slots = new ArrayList<>();
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
        // Declaration order matches resolver slot indices for this scope.
        slots.add(value);
    }
    Environment ancestor(int distance) {
        Environment environment = this;
        for (int i = 0; i < distance; i++) {
            environment = environment.enclosing;
        }
        return environment;
    }
    Object getAt(int distance, int index, Token name) {
        Environment environment = ancestor(distance);
        if (index < 0 || index >= environment.slots.size()) {
            throw new RuntimeError(name,
                    "Undefined variable '" + name.lexeme + "'.");
        }
        // Local lookup uses slot index instead of a string key lookup.
        Object value = environment.slots.get(index);
        if (value == UNINITIALIZED) {
            throw new RuntimeError(name,
                    "Variable '" + name.lexeme + "' has not been initialized.");
        }
        return value;
    }
    void assignAt(int distance, int index, Token name, Object value) {
        Environment environment = ancestor(distance);
        if (index < 0 || index >= environment.slots.size()) {
            throw new RuntimeError(name,
                    "Undefined variable '" + name.lexeme + "'.");
        }
        // Update slot first; map is kept in sync for existing name-based paths.
        environment.slots.set(index, value);
        environment.values.put(name.lexeme, value);
    }
    Environment() {
        enclosing = null;
    }
    Environment(Environment enclosing) {
        this.enclosing = enclosing;
    }
}