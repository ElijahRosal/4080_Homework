// Elijah Rosal - CS4080 - Homework 6, Chapter 13 Question 3
// 3.12.2026
/*
Code below has been modified for Question 3 for Chapter 13 of Crafting Interpreters.

Adds a native dictionary object with common operations for real programs:
set/get/has/remove/size/clear/keys.
*/

package com.craftinginterpreters.lox;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

class LoxDictionary {
    private final Map<Object, Object> entries = new HashMap<>();
    private final Map<String, LoxCallable> methods = new HashMap<>();

    LoxDictionary() {
        methods.put("set", new LoxCallable() {
            @Override
            public int arity() {
                return 2;
            }

            @Override
            public Object call(Interpreter interpreter, List<Object> arguments) {
                entries.put(arguments.get(0), arguments.get(1));
                return arguments.get(1);
            }

            @Override
            public String toString() {
                return "<dict fn set>";
            }
        });

        methods.put("get", new LoxCallable() {
            @Override
            public int arity() {
                return 1;
            }

            @Override
            public Object call(Interpreter interpreter, List<Object> arguments) {
                return entries.getOrDefault(arguments.get(0), null);
            }

            @Override
            public String toString() {
                return "<dict fn get>";
            }
        });

        methods.put("has", new LoxCallable() {
            @Override
            public int arity() {
                return 1;
            }

            @Override
            public Object call(Interpreter interpreter, List<Object> arguments) {
                return entries.containsKey(arguments.get(0));
            }

            @Override
            public String toString() {
                return "<dict fn has>";
            }
        });

        methods.put("remove", new LoxCallable() {
            @Override
            public int arity() {
                return 1;
            }

            @Override
            public Object call(Interpreter interpreter, List<Object> arguments) {
                return entries.remove(arguments.get(0));
            }

            @Override
            public String toString() {
                return "<dict fn remove>";
            }
        });

        methods.put("size", new LoxCallable() {
            @Override
            public int arity() {
                return 0;
            }

            @Override
            public Object call(Interpreter interpreter, List<Object> arguments) {
                return (double) entries.size();
            }

            @Override
            public String toString() {
                return "<dict fn size>";
            }
        });

        methods.put("clear", new LoxCallable() {
            @Override
            public int arity() {
                return 0;
            }

            @Override
            public Object call(Interpreter interpreter, List<Object> arguments) {
                entries.clear();
                return null;
            }

            @Override
            public String toString() {
                return "<dict fn clear>";
            }
        });

        methods.put("keys", new LoxCallable() {
            @Override
            public int arity() {
                return 0;
            }

            @Override
            public Object call(Interpreter interpreter, List<Object> arguments) {
                List<String> keyStrings = new ArrayList<>();
                for (Object key : entries.keySet()) {
                    keyStrings.add(stringify(key));
                }
                return "[" + String.join(", ", keyStrings) + "]";
            }

            @Override
            public String toString() {
                return "<dict fn keys>";
            }
        });
    }

    Object get(Token name) {
        if (methods.containsKey(name.lexeme)) {
            return methods.get(name.lexeme);
        }
        throw new RuntimeError(name,
                "Undefined dictionary member '" + name.lexeme + "'.");
    }

    private String stringify(Object object) {
        if (object == null) return "nil";
        if (object instanceof Double) {
            String text = object.toString();
            if (text.endsWith(".0")) {
                text = text.substring(0, text.length() - 2);
            }
            return text;
        }
        return object.toString();
    }

    @Override
    public String toString() {
        return "<dict " + entries.toString() + ">";
    }
}
