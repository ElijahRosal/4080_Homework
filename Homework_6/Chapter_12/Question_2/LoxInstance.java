// Elijah Rosal - CS4080 - Homework 6, Chapter 12 Question 2
// 3.12.2026
/*
Code below has been modified for Question 2 for Chapter 12 of Crafting Interpreters.

Executes getter methods on property reads while preserving field and normal
method behavior.
*/

package com.craftinginterpreters.lox;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;
class LoxInstance {
    private LoxClass klass;
    private final Map<String, Object> fields = new HashMap<>();
    LoxInstance(LoxClass klass) {
        this.klass = klass;
    }
    Object get(Token name, Interpreter interpreter) {
        if (fields.containsKey(name.lexeme)) {
            return fields.get(name.lexeme);
        }
        LoxFunction method = klass.findMethod(name.lexeme);
        if (method != null) {
            LoxFunction boundMethod = method.bind(this);
            if (method.isGetter()) {
                return boundMethod.call(interpreter, new ArrayList<>());
            }
            return boundMethod;
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