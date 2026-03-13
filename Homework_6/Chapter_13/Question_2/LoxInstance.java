// Elijah Rosal - CS4080 - Homework 6, Chapter 13 Question 2
// 3.12.2026
/*
Code below has been modified for Question 2 for Chapter 13 of Crafting Interpreters.

Exposes the runtime class of an instance so inner() can find the next
refinement method down the hierarchy.
*/

package com.craftinginterpreters.lox;
import java.util.HashMap;
import java.util.Map;
class LoxInstance {
    private LoxClass klass;
    private final Map<String, Object> fields = new HashMap<>();
    LoxInstance(LoxClass klass) {
        this.klass = klass;
    }

    LoxClass getKlass() {
        return klass;
    }

    Object get(Token name) {
        if (fields.containsKey(name.lexeme)) {
            return fields.get(name.lexeme);
        }
        LoxFunction method = klass.findMethod(name.lexeme);
        if (method != null) return method.bind(this);
        throw new RuntimeError(name,
                "Undefined property '" + name.lexeme + "'.");
    }
    void set(Token name, Object value) {
        fields.put(name.lexeme, value);
    }
    @Override
    public String toString() {
        return klass.name + " instance";
    }
}