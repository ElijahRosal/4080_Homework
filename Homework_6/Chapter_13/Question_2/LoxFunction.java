// Elijah Rosal - CS4080 - Homework 6, Chapter 13 Question 2
// 3.12.2026
/*
Code below has been modified for Question 2 for Chapter 13 of Crafting Interpreters.

Captures method context (declaring class and bound receiver) so inner()
can dispatch to the next refinement method down the inheritance chain.
*/

package com.craftinginterpreters.lox;
import java.util.List;
class LoxFunction implements LoxCallable {
    private final Stmt.Function declaration;
    private final Environment closure;
    private final boolean isInitializer;
    private final LoxClass containingClass;
    private final LoxInstance boundInstance;

    LoxFunction(Stmt.Function declaration, Environment closure,
                boolean isInitializer, LoxClass containingClass,
                LoxInstance boundInstance) {
        this.isInitializer = isInitializer;
        this.closure = closure;
        this.declaration = declaration;
        this.containingClass = containingClass;
        this.boundInstance = boundInstance;
    }

    LoxFunction(Stmt.Function declaration, Environment closure, boolean isInitializer) {
        this(declaration, closure, isInitializer, null, null);
    }

    LoxFunction bind(LoxInstance instance) {
        Environment environment = new Environment(closure);
        environment.define("this", instance);
        return new LoxFunction(declaration, environment, isInitializer,
                containingClass, instance);
    }

    LoxClass getContainingClass() {
        return containingClass;
    }

    LoxInstance getBoundInstance() {
        return boundInstance;
    }

    String getMethodName() {
        return declaration.name.lexeme;
    }

    @Override
    public Object call(Interpreter interpreter,
                       List<Object> arguments) {
        interpreter.pushCurrentMethod(this);
        Environment environment = new Environment(closure);
        for (int i = 0; i < declaration.params.size(); i++) {
            environment.define(declaration.params.get(i).lexeme, arguments.get(i));
        }
        try {
            interpreter.executeBlock(declaration.body, environment);
        } catch (Return returnValue) {
            interpreter.popCurrentMethod();
            if (isInitializer) return closure.getAt(0, "this");
            return returnValue.value;
        }
        interpreter.popCurrentMethod();
        if (isInitializer) return closure.getAt(0, "this");
        return null;
    }
    @Override
    public int arity() {
        return declaration.params.size();
    }
    @Override
    public String toString() {
        return "<fn " + declaration.name.lexeme + ">";
    }
}