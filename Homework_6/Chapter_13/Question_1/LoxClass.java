// Elijah Rosal - CS4080 - Homework 6, Chapter 13 Question 1
// 3.12.2026
/*
Code below has been modified for Question 1 for Chapter 13 of Crafting Interpreters.

Adds mixin support so included mixin methods are checked before the superclass
during method lookup: class own methods → mixins (in order) → superclass.
*/

package com.craftinginterpreters.lox;
import java.util.List;
import java.util.Map;
class LoxClass implements LoxCallable {
    final String name;
    final LoxClass superclass;
    private final List<LoxMixin> mixins;
    private final Map<String, LoxFunction> methods;
    LoxClass(String name, LoxClass superclass, List<LoxMixin> mixins,
             Map<String, LoxFunction> methods) {
        this.superclass = superclass;
        this.mixins = mixins;
        this.name = name;
        this.methods = methods;
    }
    LoxFunction findMethod(String name) {
        if (methods.containsKey(name)) {
            return methods.get(name);
        }
        for (LoxMixin mixin : mixins) {
            LoxFunction method = mixin.findMethod(name);
            if (method != null) return method;
        }
        if (superclass != null) {
            return superclass.findMethod(name);
        }
        return null;
    }
    @Override
    public String toString() {
        return name;
    }
    @Override
    public Object call(Interpreter interpreter,
                       List<Object> arguments) {
        LoxInstance instance = new LoxInstance(this);
        LoxFunction initializer = findMethod("init");
        if (initializer != null) {
            initializer.bind(instance).call(interpreter, arguments);
        }
        return instance;
    }
    @Override
    public int arity() {
        LoxFunction initializer = findMethod("init");
        if (initializer == null) return 0;
        return initializer.arity();
    }
}
