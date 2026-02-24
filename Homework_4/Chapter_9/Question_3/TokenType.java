// Elijah Rosal - CS4080 - Homework 4, Chapter 9 Question 3
// 2.23.2026
/*
Code below has been modified for Question 3 for Chapter 9 of Crafting Interpreters.

Adds the BREAK token type to support break statements.
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
    AND, BREAK, CLASS, ELSE, FALSE, FUN, FOR, IF, NIL, OR,
    PRINT, RETURN, SUPER, THIS, TRUE, VAR, WHILE,
    EOF
}
