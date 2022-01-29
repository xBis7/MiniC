#pragma once
#include <map>
#include <string>
#include "CompositeConcrete.h"
using namespace std;

class STNode;
class CIDENTIFIER;

class Symbol {
public:
	string m_name;
	double value;
	CIDENTIFIER* id;
	STNode* syntaxNode;
};

class SymbolTable {
public:
	map<string, Symbol*>* m_symbolTable;

	SymbolTable() {
		m_symbolTable = new map<string, Symbol*>;
	}

	virtual ~SymbolTable() {}

	Symbol* GetIdentifier(string name);
};

extern SymbolTable g_symbolTable;
