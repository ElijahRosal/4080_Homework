// Elijah Rosal - CS4080 - Homework 5, Chapter 10 Question 2
// 2.25.2026
/*
Code below has been modified for Question 2 for Chapter 10 of Crafting Interpreters.

Extends function objects to support both named function declarations and
anonymous function expression declarations.
*/

package com.craftinginterpreters.lox;
import java.util.List;
class LoxFunction implements LoxCallable {
    private final List<Token> params;
    private final List<Stmt> body;
    // CH10 Q2 CHANGE: `name` is null for anonymous function expressions.
    private final String name;
    private final Environment closure;

    LoxFunction(Stmt.Function declaration, Environment closure) {
        this.params = declaration.params;
        this.body = declaration.body;
        this.name = declaration.name.lexeme;
        this.closure = closure;
    }

    // CH10 Q2 CHANGE: Constructor overload for anonymous Expr.Function nodes.
    LoxFunction(Expr.Function declaration, Environment closure) {
        this.params = declaration.params;
        this.body = declaration.body;
        this.name = null;
        this.closure = closure;
    }

    @Override
    public Object call(Interpreter interpreter,
                       List<Object> arguments) {
        Environment environment = new Environment(closure);
        for (int i = 0; i < params.size(); i++) {
            environment.define(params.get(i).lexeme,
                    arguments.get(i));
        }
        try {
            interpreter.executeBlock(body, environment);
        } catch (Return returnValue) {
            return returnValue.value;
        }
        return null;
    }
    @Override
    public int arity() {
        return params.size();
    }
    @Override
    public String toString() {
        // CH10 Q2 CHANGE: Anonymous functions print without a name.
        if (name == null) return "<fn>";
        return "<fn " + name + ">";
    }
}