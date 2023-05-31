#include <stdio.h>
#include <stdlib.h>
#include "minako.h"

void program();

void functiondefinition();

void functioncall();

void statementlist();

void block();

void statement();

void ifstatement();

void returnstatement();

void myprintf();

void type();

void statassignment();

void assignment();

void expr();

void simpexpr();

void term();

void factor();

void error();

int currentToken, nextToken;

int main(int argc, char *argv[]) {

    if (argc != 2) {
        yyin = stdin;
    } else {
        yyin = fopen(argv[1], "r");
        if (yyin == 0) {
            fprintf(stderr, "Fehler: Konnte Datei %s nicht zum lesen oeffnen.\n", argv[1]);
            exit(-1);
        }
    }
    /*
    nextToken = getNextToken();
    program();	*/
    return 0;
}

int getLookahead() {
    int lookahead = yylex();
    return lookahead;
}

void advance() {
    currentToken = nextToken;
    nextToken = getLookahead();
}

void eat(int token) {
    advance();
}

int isToken(int token) {
    if (nextToken == token) {
        return 1;
    }
    return 0;
}

void isTokenAndEat(int token) {
    if (isToken(token) == 1) {
        eat(token);
    } else {
        error();
    }
}

void error() {
    //TODO error msg with line && exit(-1)
}

void program() {
    switch (nextToken) {

        case KW_BOOLEAN:
        case KW_FLOAT:
        case KW_INT:
        case KW_VOID:
            type();
            isTokenAndEat(ID);
            isTokenAndEat('(');
            isTokenAndEat(')');
            isTokenAndEat('{');
            statementlist();
            isTokenAndEat('}');
            program();
            break;
        case EOF:
            break;
        default:
            error();
            break;

    }


}       //TODO rework

void functiondefinition() {
    type();
    isTokenAndEat(ID);
    isTokenAndEat('(');
    isTokenAndEat(')');
    isTokenAndEat('{');
    statementlist();
    isTokenAndEat('}');

}

void functioncall() {
    isTokenAndEat(ID);
    isTokenAndEat('(');
    isTokenAndEat(')');
}

void statementlist(){



}
