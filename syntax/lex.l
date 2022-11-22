%{
    #include <stdio.h>
    #include "y.tab.h"
    #include <string.h>    
%}
%option noyywrap
%option yylineno
%%

"nop"   {return NOP;}
"mov"   {return MOV;}
"spc"   {return SPC;}
"jfg"   {return JFG;}
"inc"   {return INC;}
"dec"   {return DEC;}
"not"   {return NOT;}
"rot"   {return ROT;}
"and"   {return AND;}
"or"    {return OR; }
"adc"   {return ADC;}
"sbb"   {return SBB;}
"mul"   {return MUL;}
"div"   {return DIV;}
"mov16" {return MOV16;}
"add16" {return ADD16;}
"rga"
"rgb"
"rgc"
"rgd"
"rmh"
"rml"
"@"     {return VALUE_AT;}
-?([1-9][0-9]*)|0 {yylval.intval = atoi(yytext); return IMM8;}


[ \t] ;
\n {yylineno++;}
\r ;

%%