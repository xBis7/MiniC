
%language "C++"

%{
#include <iostream>
#include "MiniC.tab.h"
#include "CompositeConcrete.h"
#include "SymbolTable.h"

using namespace std;

extern int yylex(yy::parser::semantic_type *yylval);
extern FILE *yyin;

%}

%error-verbose

%code requires {
	#include "Composite.h"
}

%union {
	STNode *node;
}

%start compileUnit

%token <node> NUMBER IDENTIFIER
%token IF ELSE WHILE RETURN BREAK FUNCTION

%nonassoc IFPREC

%right '='

%left OR 
%left AND 

%nonassoc GTE GT LTE LT NEQUAL EQUAL 

%left '+' '-'
%left '*' '/'

%nonassoc ELSE
%nonassoc NOT

%type <node> compileUnit functionDef fargs statement compoundStatement ifStatement whileStatement statementList expression args

%%

compileUnit : statement							{ $$ = g_root = new CCompileUnit($1); }	
			| compileUnit statement				{ $$ = g_root = new CCompileUnit($1, $2); }
			| functionDef						{ $$ = g_root = new CCompileUnit($1); }
			| compileUnit functionDef			{ $$ = g_root = new CCompileUnit($1, $2); }
			;

functionDef : FUNCTION IDENTIFIER '(' fargs ')' compoundStatement		{	$$ = new CFunctionDef($2, $4, $6); 
																			g_symbolTable.GetIdentifier(((CIDENTIFIER *)$2)->m_name)->syntaxNode =$$;
																		}
			| FUNCTION IDENTIFIER '(' ')' compoundStatement				{	$$ = new CFunctionDef($2, $5); 
																			g_symbolTable.GetIdentifier(((CIDENTIFIER *)$2)->m_name)->syntaxNode =$$;
																		}
			;
				
fargs : IDENTIFIER								{ $$ = new CFargs($1); }
		| fargs ',' IDENTIFIER					{ $$ = new CFargs($1, $3); }
		;

statement : expression ';'						{ $$ = new CExpStatement($1); }
			| compoundStatement					
			| ifStatement						
			| whileStatement					
			| RETURN expression ';'				{ $$ = new CReturnStatement($2); }
			| BREAK ';'							{ $$ = new CBreakStatement(); }
			| ';'								{ $$ = new CEmptyStatement(); }
			;


compoundStatement : '{' '}'						{ $$ = new CCompoundStatement(); }
					| '{' statementList '}'		{ $$ = new CCompoundStatement($2); }
					;

ifStatement : IF '(' expression ')' statement %prec IFPREC			{ $$ = new CIfStatement($3, $5); }
			| IF '(' expression ')' statement ELSE statement		{ $$ = new CIfStatement($3, $5, $7); }
			;

whileStatement : WHILE '(' expression ')' statement			{ $$ = new CWhileStatement($3, $5); }
				;

statementList : statement								{ $$ = new CStatementList($1); }
				| statementList statement				{ $$ = new CStatementList($1, $2); }
				;

expression : NUMBER									{ $$ = $1; }
			| IDENTIFIER							{	$$ = new CExpIdentifier($1); 
														g_symbolTable.GetIdentifier(((CIDENTIFIER *)$1)->m_name)->syntaxNode =$$;
													}
			| IDENTIFIER '(' ')'					{ $$ = new CFunctionCall($1); }
			| IDENTIFIER '(' args ')'				{ $$ = new CFunctionCall($1, $3); }
			| '(' expression ')'					{ $$ = $2; }
			| IDENTIFIER '=' expression				{ $$ = new CAssignment($1, $3); }
			| expression '+' expression				{ $$ = new CAddition($1, $3); }
			| expression '-' expression				{ $$ = new CSubtraction($1, $3); }
			| expression '*' expression				{ $$ = new CMultiplication($1, $3); }
			| expression '/' expression				{ $$ = new CDivision($1, $3); }
			| '+' expression						{ $$ = new CUnaryPlus($2); }
			| '-' expression						{ $$ = new CUnaryMinus($2); }
			| NOT expression						{ $$ = new CExpNot($2); }
			| expression AND expression				{ $$ = new CExpAnd($1, $3); }
			| expression OR expression				{ $$ = new CExpOr($1, $3); }
			| expression NEQUAL expression			{ $$ = new CExpNEqual($1, $3); }
			| expression EQUAL expression			{ $$ = new CExpEqual($1, $3); }
			| expression LT expression				{ $$ = new CExpLT($1, $3); }
			| expression GT expression				{ $$ = new CExpGT($1, $3); }
			| expression LTE expression				{ $$ = new CExpLTE($1, $3); }
			| expression GTE expression				{ $$ = new CExpGTE($1, $3); }
			;

args : expression						{ $$ = new CArgs($1); }
		| args ',' expression			{ $$ = new CArgs($1, $3); }
		;

%%

namespace yy{
	void parser::error(yy::location const &loc, const string &message){
		std::cerr << "error at " << loc << ": " << message << std::endl;
	}
}

