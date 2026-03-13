// Elijah Rosal - CS4080 - Homework 6, Chapter 13 Question 2
// 3.12.2026
/*
Code below has been modified for Question 2 for Chapter 13 of Crafting Interpreters.

Implements BETA-style method lookup: top-most matching method wins, and
inner() can chain to the nearest refining method down the hierarchy.
*/

package com.craftinginterpreters.lox;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
class LoxClass implements LoxCallable {
    final String name;
    final LoxClass superclass;
    private final Map<String, LoxFunction> methods;
    LoxClass(String name, LoxClass superclass, Map<String, LoxFunction> methods) {
        this.superclass = superclass;
        this.name = name;
        this.methods = methods;
    }
    LoxFunction findMethod(String name) {
        if (superclass != null) {
            LoxFunction inherited = superclass.findMethod(name);
            if (inherited != null) return inherited;
        }
        return methods.getOrDefault(name, null);
    }

    LoxFunction findOwnMethod(String name) {
        return methods.getOrDefault(name, null);
    }

    private void appendLineage(List<LoxClass> lineage) {
        if (superclass != null) superclass.appendLineage(lineage);
        lineage.add(this);
    }

    private List<LoxClass> lineageFromRoot() {
        List<LoxClass> lineage = new ArrayList<>();
        appendLineage(lineage);
        return lineage;
    }

    LoxFunction findInnerMethod(String methodName,
                                LoxClass containingClass,
                                LoxClass receiverClass) {
        List<LoxClass> lineage = receiverClass.lineageFromRoot();
        int containingIndex = lineage.indexOf(containingClass);
        if (containingIndex == -1) return null;

        for (int i = containingIndex + 1; i < lineage.size(); i++) {
            LoxFunction method = lineage.get(i).findOwnMethod(methodName);
            if (method != null) return method;
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
