%{

#include <string.h>
#include "parser.tab.h"
//#include "storetokens.hh"
int lineno = 1;

%}

ws	[ \t]
letter	[a-zA-Z]
digit	[0-9]
nonzero	[1-9]

%%

\n				{lineno++;}
{ws}			{;}
(\/\/.*)		{;}
return 			{store_token_name("RETURN", yytext, yylineno); return RETURN;}
int 			{store_token_name("INTEGER", yytext, yylineno); return INTEGER;}
float 			{store_token_name("FLOAT", yytext, yylineno); return FLOAT;}
void			{store_token_name("VOID", yytext, yylineno); return VOID;}
while			{store_token_name("WHILE", yytext, yylineno); return WHILE;}
do 				{store_token_name("DO", yytext, yylineno); return DO;}
if 				{store_token_name("IF", yytext, yylineno); return IF;}
else 			{store_token_name("ELSE", yytext, yylineno); return ELSE;}
print			{store_token_name("PRINT", yytext, yylineno); return PRINT;}
(0|{nonzero}{digit}*)			{store_token_name("NUM", yytext, yylineno); yylval.integer_value = atoi(yytext);
                                return INTEGER_NUMBER;}
({letter}|_)({letter}|{digit}|_)* 	{store_token_name("NAME", yytext, yylineno); std::string * name = new std::string(yytext); yylval.string_value = name;
                                return NAME;}
(({digit}+\.{digit}*)|({digit}*\.{digit}+))(e(-|\+)?[0-9]+)?              {store_token_name("FNUM", yytext, yylineno); yylval.double_value = atof(yytext);
                                return DOUBLE_NUMBER;}
"<="				{store_token_name("LESS_THAN_EQUAL", yytext, yylineno); return LTE;}
>=				{store_token_name("GREATER_THAN_EQUAL", yytext, yylineno); return GTE;}
==				{store_token_name("EQUAL", yytext, yylineno); return EQUAL;}
!=				{store_token_name("NOT_EQUAL", yytext, yylineno); return NE;}
= 				{store_token_name("ASSIGN_OP", yytext, yylineno); return ASSIGN;}
"<"				{store_token_name("LESS_THAN", yytext, yylineno); return LT;}
>				{store_token_name("GREATER_THAN", yytext, yylineno); return GT;}
\+|-|\*|\/      {store_token_name("ARITHOP",  yytext, yylineno); return yytext[0];}
&&				{store_token_name("AND",  yytext, yylineno); return AND;}
\|\|			{store_token_name("OR",  yytext, yylineno); return OR;}
!				{store_token_name("NOT",  yytext, yylineno); return NOT;}
.				{store_token_name("META CHAR", yytext, yylineno); return yytext[0];}


%%
