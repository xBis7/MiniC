
%option noyywrap

%{
#include <iostream>
#include <cstdlib>
#include "MiniC.tab.h"
#include "CompositeConcrete.h"
#include "SymbolTable.h"
using namespace std;

#define YY_DECL int yylex(yy::parser::semantic_type *yylval)
typedef yy::parser::token token;

%}

INT "0"|[1-9][0-9]*

%%

"if"			{ return token::IF; }
"else"			{ return token::ELSE; }
"while"			{ return token::WHILE; }
"break"			{ return token::BREAK; }
"return"		{ return token::RETURN; }
"function"		{ return token::FUNCTION; }

","		{ return ','; }
";"		{ return ';'; }
"+"		{ return '+'; } 
"-"		{ return '-'; } 
"*"		{ return '*'; } 
"/"		{ return '/'; } 
"="		{ return '='; } 
"("		{ return '('; } 
")"		{ return ')'; } 
"{"		{ return '{'; }
"}"		{ return '}'; }
"&&"	{ return token::AND; }
"||"	{ return token::OR; }
"!"		{ return token::NOT; }
"!="	{ return token::NEQUAL; }
"=="	{ return token::EQUAL; }
">="	{ return token::GTE; }
"<="	{ return token::LTE; }
"<"		{ return token::LT; }
">"		{ return token::GT; }

[a-zA-Z][a-zA-Z0-9_]*			{	yylval->node = g_symbolTable.GetIdentifier(yytext)->id;
									return token::IDENTIFIER; }

({INT}(\.){INT})				{	yylval->node = new CNUMBER(yytext, atof(yytext)); 
									return token::NUMBER; }

{INT}							{	yylval->node = new CNUMBER(yytext, (double)atoi(yytext)); 
									return token::NUMBER; }

[ \t\n\r]+ ;

%%

