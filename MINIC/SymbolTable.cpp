#include "SymbolTable.h"
#include "CompositeConcrete.h"

SymbolTable g_symbolTable;

Symbol* SymbolTable::GetIdentifier(string name) {
	map<string, Symbol*>::iterator it;

	Symbol* symbol;

	//search map
	it = m_symbolTable->find(name);

	//if no result we add it to the map
	if (it == m_symbolTable->end()) {
		symbol = new Symbol;
		symbol->m_name = name;
		symbol->id = new CIDENTIFIER(name.c_str());
		symbol->value = 0;

		(*m_symbolTable)[name] = symbol;
		return symbol;
	}
	else {
		//if result we return it
		return (*m_symbolTable)[name];
	}
	
}
