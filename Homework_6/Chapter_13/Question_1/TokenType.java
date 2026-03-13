// Elijah Rosal - CS4080 - Homework 6, Chapter 13 Question 1
// 3.12.2026
/*
Code below has been modified for Question 1 for Chapter 13 of Crafting Interpreters.

Adds MIXIN and WITH keyword tokens to support mixin declarations and the
"with" clause in class definitions.
*/

package com.craftinginterpreters.lox;
enum TokenType {
    // Single-character tokens.
    LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
    COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR,
    QUESTION, COLON,
    // One or two character tokens.
    BANG, BANG_EQUAL,
    EQUAL, EQUAL_EQUAL,
    GREATER, GREATER_EQUAL,
    LESS, LESS_EQUAL,
    // Literals.
    IDENTIFIER, STRING, NUMBER,
    // Keywords.
    AND, BREAK, CLASS, ELSE, FALSE, FUN, FOR, IF, MIXIN, NIL, OR,
    PRINT, RETURN, SUPER, THIS, TRUE, VAR, WHILE, WITH,
    EOF
}
