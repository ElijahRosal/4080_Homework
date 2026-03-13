// Elijah Rosal - CS4080 - Homework 6, Chapter 12 Question 1
// 3.12.2026
/*
Code below has been modified for Question 1 for Chapter 12 of Crafting Interpreters.

Updates instance property lookup so class-object property access remains safe
when using metaclasses for static method dispatch.
*/

package com.craftinginterpreters.lox;
import java.util.HashMap;
import java.util.Map;
class LoxInstance {
    private final LoxClass klass;
    private final Map<String, Object> fields = new HashMap<>();
    LoxInstance(LoxClass klass) {
        this.klass = klass;
    }
    Object get(Token name) {
        if (fields.containsKey(name.lexeme)) {
            return fields.get(name.lexeme);
        }
        if (klass != null) {
            LoxFunction method = klass.findMethod(name.lexeme);
            if (method != null) return method.bind(this);
        }
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