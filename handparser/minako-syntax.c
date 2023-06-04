#include <stdio.h>
#include <stdlib.h>
#include "minako.h"

int nextToken;
int currentToken;

void functionDefinition();
void functionCall();
void statementList();
void block();
void statement();
void ifStatement();
void returnStatement();
void fn_printf();
void type();
void statAssignment();
void assignment();
void expr();
void simpExpr();
void term();
void factor();

void eat() {
    currentToken = nextToken;
    nextToken = yylex();
}

int isToken(int token) {
    if(currentToken == token) {
        //char cChar = (char)currentToken;
        //printf("Current Token: %c \n", cChar);
        return 1;
    }else {
        return 0;
    }
}

void isTokenAndEat(int token) {
    if(!isToken(token)) {
        char expectedChar = (char)token;
        char currentChar = (char)currentToken;
        fprintf(stderr, "ERROR: Syntaxfehler in Zeile: %d \nExpected: %c \nCurrent: %c \n", yylineno, expectedChar, currentChar);
        exit(EXIT_FAILURE);
    }
    eat();
}

// GRAMMATIK
void factor() {
    if(isToken(CONST_INT) || isToken(CONST_BOOLEAN) || isToken(CONST_FLOAT)) {
        eat();
        return;
    }
    else if(isToken(ID)) {
        if(nextToken == '(') {
            functionCall();
            return;
        }
        eat();
        return;
    }
    else if(isToken('(')) {
        isTokenAndEat('(');
        assignment();
        isTokenAndEat(')');
        return;
    }
    else {
        printf("Error");
    }
}

void term() {
    factor();
    while(isToken('*') || isToken('/') || isToken(AND)) {
        eat();
        factor();
    }
    return;
}

void simpExpr() {
    if(isToken('-')) {
        isTokenAndEat('-');
    }
    term();
    while(isToken('+') || isToken('-') || isToken(OR)) {
        eat();
        term();
    }
    return;
}

void expr() {
    simpExpr();
    if(isToken(EQ) || isToken(NEQ) || isToken(LEQ) || isToken(GEQ) || isToken(LSS) || isToken(GRT)) {
        eat();
        simpExpr();
    }
    return;
}

void assignment() {
    if(isToken(ID) && nextToken == '=') {
        isTokenAndEat(ID);
        isTokenAndEat('=');
        assignment();
    }else {
        expr();
    }
    return;
}


void returnStatement() {
    isTokenAndEat(KW_RETURN);
    // assignment() is nicht obligatorisch, deshalb schauen wir schon in dieser Funktion, ob die nächsten Token assignment voraussetzen
    if(isToken(ID) || isToken('-') || isToken(CONST_INT) || isToken(CONST_FLOAT) || isToken(CONST_BOOLEAN) || isToken('(')) {
        assignment();
    }
    return;
}

void fn_printf() {
    isTokenAndEat(KW_PRINTF);
    isTokenAndEat('(');
    assignment();
    isTokenAndEat(')');
    return;
}

void statAssignment() {
    isTokenAndEat(ID);
    isTokenAndEat('=');
    assignment();
    return;
}

void functionCall() {
    isTokenAndEat(ID);
    isTokenAndEat('(');
    isTokenAndEat(')');
    return;
}

void statement() {
    if(isToken(KW_IF))  {
        ifStatement();
        return;
    }
    else if(isToken(KW_RETURN))  {
        returnStatement();
        isTokenAndEat(';');
        return;
    }
    else if(isToken(KW_PRINTF))   {
        fn_printf();
        isTokenAndEat(';');
        return;
    }
    else if(isToken(ID) && nextToken == '=') {
        statAssignment();
        isTokenAndEat(';');
        return;
    }
    else if(isToken(ID) && nextToken == '(') {
        functionCall();
        isTokenAndEat(';');
        return;
    } else {
        printf("Error");
    }
}

// neuer Klammernblock solange das nächste Token es impliziert
void statementList() {
    while(isToken('{') || isToken(KW_IF) || isToken(KW_RETURN) || isToken(KW_PRINTF) || isToken(ID)) {
        block();
    }
    return;
}

void block() {
    if(isToken('{')) {
        isTokenAndEat('{');
        statementList();
        isTokenAndEat('}');
    }else {
        statement();
    }
    return;
}

void ifStatement() {
    isTokenAndEat(KW_IF);
    isTokenAndEat('(');
    assignment();
    isTokenAndEat(')');
    block();
    return;
}

void type() {
    switch(currentToken) {
        case KW_BOOLEAN: isTokenAndEat(KW_BOOLEAN); break;
        case KW_FLOAT: isTokenAndEat(KW_FLOAT); break;
        case KW_INT: isTokenAndEat(KW_INT); break;
        case KW_VOID: isTokenAndEat(KW_VOID); break;
    }
}

void functionDefinition() {
    type();
    isTokenAndEat(ID);
    isTokenAndEat('(');
    isTokenAndEat(')');
    isTokenAndEat('{');
    statementList();
    isTokenAndEat('}');
    return;
}

void program() {
    while(isToken(EOF) == 0) {
        functionDefinition();
    }
    return;
}

int main(int argc, char** argv) {
    FILE* input;
    if(argc <= 1) {
        input = stdin;
    }else {
        input = fopen(argv[1],"r");
    }
    yyin = input;
    currentToken = yylex();
    nextToken = yylex();
    program();

    if(input != stdin) {
        fclose(input);
    }
    return EXIT_SUCCESS;
}
