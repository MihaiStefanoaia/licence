%{
#include <stdio.h>
#include <string>
extern FILE* yyin
extern char* yytext;
extern int yylineno;
%}

%union {
    unsigned char intval;
    char* strval;
}

%token <strval>NOP MOV SPC JFG INC DEC NOT ROT AND OR ADC SBB MUL DIV MOV16 ADD16 

%%

%%