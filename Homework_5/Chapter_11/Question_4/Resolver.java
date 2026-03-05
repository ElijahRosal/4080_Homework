package com.craftinginterpreters.lox;
// Elijah Rosal - CS4080 - Homework 5, Chapter 11 Question 4
// 3.5.2026
/*
Code below has been modified for Question 4 for Chapter 11 of Crafting Interpreters.

Extends resolver scope tracking to assign each local declaration a unique slot index
and resolves variable accesses with both lexical depth and slot index metadata.
*/

import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Stack;
class Resolver implements Expr.Visitor<Void>, Stmt.Visitor<Void> {
    private final Interpreter interpreter;
    private final Stack<Scope> scopes = new Stack<>();
    private FunctionType currentFunction = FunctionType.NONE;

    private static class Scope {
        final Map<String, VariableState> variables = new HashMap<>();
        // Next slot id to assign in this lexical scope.
        int nextIndex = 0;
    }

    private static class VariableState {
        final Token name;
        // Stable local slot index used by runtime Environment slots.
        final int index;
        boolean defined;
        boolean used;

        VariableState(Token name, int index, boolean defined) {
            this.name = name;
            this.index = index;
            this.defined = defined;
            this.used = false;
        }
    }

    Resolver(Interpreter interpreter) {
        this.interpreter = interpreter;
    }
    private enum FunctionType {
        NONE,
        FUNCTION
    }
    @Override
    public Void visitBlockStmt(Stmt.Block stmt) {
        beginScope();
        resolve(stmt.statements);
        endScope();
        return null;
    }
    @Override
    public Void visitExpressionStmt(Stmt.Expression stmt) {
        resolve(stmt.expression);
        return null;
    }
    @Override
    public Void visitBreakStmt(Stmt.Break stmt) {
        return null;
    }
    @Override
    public Void visitFunctionStmt(Stmt.Function stmt) {
        declare(stmt.name);
        define(stmt.name);
        resolveFunction(stmt, FunctionType.FUNCTION);
        return null;
    }
    @Override
    public Void visitIfStmt(Stmt.If stmt) {
        resolve(stmt.condition);
        resolve(stmt.thenBranch);
        if (stmt.elseBranch != null) resolve(stmt.elseBranch);
        return null;
    }
    @Override
    public Void visitPrintStmt(Stmt.Print stmt) {
        resolve(stmt.expression);
        return null;
    }
    @Override
    public Void visitReturnStmt(Stmt.Return stmt) {
        if (currentFunction == FunctionType.NONE) {
            Lox.error(stmt.keyword,
                    "Can't return from top-level code.");
        }
        if (stmt.value != null) {
            resolve(stmt.value);
        }
        return null;
    }
    @Override
    public Void visitVarStmt(Stmt.Var stmt) {
        declare(stmt.name);
        if (stmt.initializer != null) {
            resolve(stmt.initializer);
        }
        define(stmt.name);
        return null;
    }
    @Override
    public Void visitWhileStmt(Stmt.While stmt) {
        resolve(stmt.condition);
        resolve(stmt.body);
        return null;
    }
    @Override
    public Void visitVariableExpr(Expr.Variable expr) {
        if (!scopes.isEmpty() &&
            scopes.peek().variables.containsKey(expr.name.lexeme) &&
            !scopes.peek().variables.get(expr.name.lexeme).defined)
        {
            Lox.error(expr.name,

                    "Can't read local variable in its own initializer.");
        }
        resolveLocal(expr, expr.name, true);
        return null;
    }
    @Override
    public Void visitAssignExpr(Expr.Assign expr) {
        resolve(expr.value);
        resolveLocal(expr, expr.name, false);
        return null;
    }
    @Override
    public Void visitBinaryExpr(Expr.Binary expr) {
        resolve(expr.left);
        resolve(expr.right);
        return null;
    }
    @Override
    public Void visitCallExpr(Expr.Call expr) {
        resolve(expr.callee);
        for (Expr argument : expr.arguments) {
            resolve(argument);
        }
        return null;
    }
    @Override
    public Void visitConditionalExpr(Expr.Conditional expr) {
        resolve(expr.condition);
        resolve(expr.thenBranch);
        resolve(expr.elseBranch);
        return null;
    }
    @Override
    public Void visitFunctionExpr(Expr.Function expr) {
        beginScope();
        for (Token param : expr.params) {
            declare(param);
            define(param);
        }
        resolve(expr.body);
        endScope();
        return null;
    }
    @Override
    public Void visitGroupingExpr(Expr.Grouping expr) {
        resolve(expr.expression);
        return null;
    }
    @Override
    public Void visitLiteralExpr(Expr.Literal expr) {
        return null;
    }
    @Override
    public Void visitLogicalExpr(Expr.Logical expr) {
        resolve(expr.left);
        resolve(expr.right);
        return null;
    }
    @Override
    public Void visitUnaryExpr(Expr.Unary expr) {
        resolve(expr.right);
        return null;
    }
        void resolve(List<Stmt> statements) {
        for (Stmt statement : statements) {
            resolve(statement);
        }
    }
    private void resolve(Stmt stmt) {
        stmt.accept(this);
    }
    private void resolve(Expr expr) {
        expr.accept(this);
    }
    private void resolveFunction(Stmt.Function function, FunctionType type) {
        FunctionType enclosingFunction = currentFunction;
        currentFunction = type;
        beginScope();
        for (Token param : function.params) {
            declare(param);
            define(param);
        }
        resolve(function.body);
        endScope();
        currentFunction = enclosingFunction;
    }
    private void beginScope() {
        scopes.push(new Scope());
    }
    private void endScope() {
        Scope scope = scopes.peek();
        for (VariableState variable : scope.variables.values()) {
            if (variable.defined && !variable.used) {
                Lox.error(variable.name,
                        "Local variable is never used.");
            }
        }
        scopes.pop();
    }
    private void declare(Token name) {
        if (scopes.isEmpty()) return;
        Scope scope = scopes.peek();
        if (scope.variables.containsKey(name.lexeme)) {
            Lox.error(name,
                    "Already a variable with this name in this scope.");
        }
        // Assign one unique slot per local declaration in this scope.
        int index = scope.nextIndex;
        scope.nextIndex++;
        scope.variables.put(name.lexeme, new VariableState(name, index, false));
    }
    private void define(Token name) {
        if (scopes.isEmpty()) return;
        scopes.peek().variables.get(name.lexeme).defined = true;
    }
    private void resolveLocal(Expr expr, Token name, boolean markUsed) {
        for (int i = scopes.size() - 1; i >= 0; i--) {
            Scope scope = scopes.get(i);
            if (scope.variables.containsKey(name.lexeme)) {
                VariableState variable = scope.variables.get(name.lexeme);
                if (markUsed) {
                    variable.used = true;
                }
                // Resolver records both lexical depth and slot index.
                interpreter.resolve(expr, scopes.size() - 1 - i, variable.index);
                return;
            }
        }
    }


    }
