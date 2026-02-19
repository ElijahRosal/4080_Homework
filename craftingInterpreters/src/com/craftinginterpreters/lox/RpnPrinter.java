package com.craftinginterpreters.lox;

class RpnPrinter implements Expr.Visitor<String> {
    String print(Expr expr) {
        return expr.accept(this);
    }

    @Override
    public String visitBinaryExpr(Expr.Binary expr) {
        // In RPN: left operand, right operand, operator
        return expr.left.accept(this) + " " +
                expr.right.accept(this) + " " +
                expr.operator.lexeme;
    }

    @Override
    public String visitConditionalExpr(Expr.Conditional expr) {
        return expr.condition.accept(this) + " " +
                expr.thenBranch.accept(this) + " " +
                expr.elseBranch.accept(this) + " ?:";
    }

    @Override
    public String visitGroupingExpr(Expr.Grouping expr) {
        // Grouping doesn't need special notation in RPN
        // Just return the inner expression
        return expr.expression.accept(this);
    }

    @Override
    public String visitLiteralExpr(Expr.Literal expr) {
        if (expr.value == null) return "nil";
        return expr.value.toString();
    }

    @Override
    public String visitUnaryExpr(Expr.Unary expr) {
        // In RPN: operand, operator
        return expr.right.accept(this) + " " + expr.operator.lexeme;
    }

    public static void main(String[] args) {
        // Test: (1 + 2) * (4 - 3)
        // Expected RPN: 1 2 + 4 3 - *
        Expr expression = new Expr.Binary(
                new Expr.Grouping(
                        new Expr.Binary(
                                new Expr.Literal(1),
                                new Token(TokenType.PLUS, "+", null, 1),
                                new Expr.Literal(2))),
                new Token(TokenType.STAR, "*", null, 1),
                new Expr.Grouping(
                        new Expr.Binary(
                                new Expr.Literal(4),
                                new Token(TokenType.MINUS, "-", null, 1),
                                new Expr.Literal(3))));

        System.out.println("Expression: (1 + 2) * (4 - 3)");
        System.out.println("RPN: " + new RpnPrinter().print(expression));

        // Test with unary: -123 * 45.67
        Expr expression2 = new Expr.Binary(
                new Expr.Unary(
                        new Token(TokenType.MINUS, "-", null, 1),
                        new Expr.Literal(123)),
                new Token(TokenType.STAR, "*", null, 1),
                new Expr.Literal(45.67));

        System.out.println("\nExpression: -123 * 45.67");
        System.out.println("RPN: " + new RpnPrinter().print(expression2));
    }
}