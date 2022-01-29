#include "CompositeConcrete.h"
#include <iostream>

#include "SymbolTable.h"
using namespace std;

CCompileUnit::CCompileUnit(STNode* arg1) : STNode(NodeType::COMPILEUNIT, 1, arg1) {}
CCompileUnit::CCompileUnit(STNode* arg1, STNode* arg2) : STNode(NodeType::COMPILEUNIT, 2, arg1, arg2) {}
CCompileUnit::~CCompileUnit() {}

CFunctionDef::CFunctionDef(STNode* id, STNode* fargs, STNode* stat) : STNode(NodeType::FUNCTIONDEF, 3, id, fargs, stat) {}
CFunctionDef::CFunctionDef(STNode* id, STNode* stat) : STNode(NodeType::FUNCTIONDEF, 2, id, stat) {}
CFunctionDef::~CFunctionDef() {}

CStatementList::CStatementList(STNode* arg1) : STNode(NodeType::STATEMENT_LIST, 1, arg1) {}
CStatementList::CStatementList(STNode* arg1, STNode* arg2) : STNode(NodeType::STATEMENT_LIST, 2, arg1, arg2) {}
CStatementList::~CStatementList() {}

CFargs::CFargs(STNode* id) : STNode(NodeType::FARGS, 1, id) {}
CFargs::CFargs(STNode* fargs, STNode* id) : STNode(NodeType::FARGS, 2, fargs, id) {}
CFargs::~CFargs() {}

CArgs::CArgs(STNode* exp) : STNode(NodeType::ARGS, 1, exp) {}
CArgs::CArgs(STNode* args, STNode* exp) : STNode(NodeType::ARGS, 2, args, exp) {}
CArgs::~CArgs() {}

CStatement::CStatement(NodeType type) : STNode(type, 0) {}
CStatement::CStatement(NodeType type, int num) : STNode(type, num) {}
CStatement::CStatement(NodeType type, int num, STNode* arg) : STNode(type, num, arg) {}
CStatement::CStatement(NodeType type, int num, STNode* arg1, STNode* arg2) : STNode(type, num, arg1, arg2) {}
CStatement::CStatement(NodeType type, int num, STNode* arg1, STNode* arg2, STNode* arg3) : STNode(type, num, arg1, arg2, arg3) {}
CStatement::~CStatement() {}

CExpStatement::CExpStatement(STNode* arg) : CStatement(NodeType::STATEMENT_EXP, 1, arg) {}
CExpStatement::~CExpStatement() {}

double CExpStatement::Visit_Eval(STNode* parent) {

	STNode* child = (STNode*)GetChild(0);

	double result = child->Visit_Eval(this);

	if (child->GetNodeType() != EXP_ASSIGNMENT) {
		cout << "Result: " << result << endl;
	}
	
	return 0;
}

CEmptyStatement::CEmptyStatement() : CStatement(NodeType::EMPTY_STATEMENT, 0) {}
CEmptyStatement::~CEmptyStatement() {}

CCompoundStatement::CCompoundStatement() : CStatement(NodeType::COMPOUND_STATEMENT, 0) {}
CCompoundStatement::CCompoundStatement(STNode* stat) : CStatement(NodeType::COMPOUND_STATEMENT, 1, stat) {}
CCompoundStatement::~CCompoundStatement() {}

CIfStatement::CIfStatement(STNode* exp, STNode* stat) : CStatement(NodeType::IF_STATEMENT, 2, exp, stat) {}
CIfStatement::CIfStatement(STNode* exp, STNode* stat, STNode* stat2) : CStatement(NodeType::IF_STATEMENT, 3, exp, stat, stat2) {}
CIfStatement::~CIfStatement() {}

double CIfStatement::Visit_Eval(STNode* parent) {
	int argsNum = m_children->size();

	if (argsNum == 2) {
		if (GetChild(0)->Visit_Eval(this)) {
			GetChild(1)->Visit_Eval(this);
		}
	}

	if (argsNum == 3) {
		if (GetChild(0)->Visit_Eval(this)) {
			GetChild(1)->Visit_Eval(this);
		}
		else {
			GetChild(2)->Visit_Eval(this);
		}
	}
	return 0;
}

CWhileStatement::CWhileStatement(STNode* exp, STNode* compStat) : CStatement(NodeType::WHILE_STATEMENT, 2, exp, compStat) {}
CWhileStatement::~CWhileStatement() {}

double CWhileStatement::Visit_Eval(STNode* parent) {
	while (GetChild(0)->Visit_Eval(this)) {

		GetChild(1)->Visit_Eval(this);

		if (m_endVisit == true) {
			m_endVisit = false;
			break;
		}
	}
	return 0;
}

CBreakStatement::CBreakStatement() : CStatement(NodeType::BREAK_STATEMENT, 0) {}
CBreakStatement::~CBreakStatement() {}

double CBreakStatement::Visit_Eval(STNode* parent) {
	m_endVisit = true;
	return 0;
}

CReturnStatement::CReturnStatement(STNode* exp) : CStatement(NodeType::RETURN_STATEMENT, 1, exp) {}
CReturnStatement::~CReturnStatement() {}

double CReturnStatement::Visit_Eval(STNode* parent) {
	m_endVisit = true;
	m_returnVal = GetChild(0)->Visit_Eval(this);
	//cout << "return val: " << m_returnVal << endl;
	return m_returnVal;
}

CExpression::CExpression(NodeType type) : STNode(type, 0) {}
CExpression::CExpression(NodeType type, int num, STNode* arg) : STNode(type, num, arg) {}
CExpression::CExpression(NodeType type, int num, STNode* arg1, STNode* arg2) : STNode(type, num, arg1, arg2) {}
CExpression::~CExpression() {}

CFunctionCall::CFunctionCall(STNode* id) : CExpression(NodeType::FUNCTION_CALL, 1, id) {}
CFunctionCall::CFunctionCall(STNode* id, STNode* args) : CExpression(NodeType::FUNCTION_CALL, 2, id, args) {}
CFunctionCall::~CFunctionCall() {}

//friend function from composite.h
void GetFArgs(STNode* node, list<CIDENTIFIER*>* fargs) {
	list<STNode*>::iterator it;

	if(dynamic_cast<CIDENTIFIER*>(node) != nullptr) {
		fargs->push_back((CIDENTIFIER*)node);
	}

	for(it = node->m_children->begin(); it != node->m_children->end(); it++) {
		GetFArgs((*it), fargs);
	}
}

//friend function from composite.h
void GetArgs(STNode* node, list<CExpression*>* args) {
	list<STNode*>::iterator it;

	if (dynamic_cast<CExpression*>(node) != nullptr) {
		args->push_back((CExpression*)node);
	}

	for (it = node->m_children->begin(); it != node->m_children->end(); it++) {
		GetArgs((*it), args);
	}
}

//actualToFormalArgs from CFunctionCall class
void CFunctionCall::actualToFormalArgs() {

	//FunctionCall Child 0 STNode *id
	CIDENTIFIER* funcId = (CIDENTIFIER*)GetChild(0);

	//actual arguments from FunctionCall
	CArgs* args = (CArgs*)GetChild(1);
	
	CFunctionDef* funcDef = (CFunctionDef*)g_symbolTable.GetIdentifier(funcId->m_name)->syntaxNode;

	//formal arguments from FunctionDef
	CFargs* fargs = (CFargs*)funcDef->GetChild(1);

	list<CIDENTIFIER*>::iterator itId;
	list<CExpression*>::iterator itExp;

	m_args = new list<CExpression*>();
	m_fargs = new list<CIDENTIFIER*>();

	GetArgs(args, m_args);
	GetFArgs(fargs, m_fargs);

	for (itId = m_fargs->begin(), itExp = m_args->begin(); itId != m_fargs->end(); itId++, itExp++) {
		g_symbolTable.GetIdentifier((*itId)->m_name)->value = (*itExp)->Visit_Eval(this);
	}
}

double CFunctionCall::GetActualArg(int index) {

	//child 0 function identifier
	//child 1 arg
	//get arg
	CArgs* args = (CArgs*)GetChild(1);

	//iterator
	list<CExpression*>::iterator it;

	//if arg list empty
	if (m_args == nullptr) {
		m_args = new list<CExpression*>;
		GetArgs(args, m_args);
	}

	it = m_args->begin();
	advance(it, index);
	return (*it)->Visit_Eval(this);
}

double CFunctionCall::Visit_Eval(STNode* parent) {

	double result = 0;
	CIDENTIFIER* funcId = (CIDENTIFIER*)GetChild(0);
	CFunctionDef* funcDef;

	if (!funcId->m_name.compare("sqrt")) {
		result = sqrt(GetActualArg(0));
	}
	else if (!funcId->m_name.compare("pow")) {
		result = pow(GetActualArg(0), GetActualArg(1));
	}
	else {
		m_insideFuncDef = true;
		//get function identifier from symbol table
		funcDef = (CFunctionDef*)g_symbolTable.GetIdentifier(funcId->m_name)->syntaxNode;
		 
		actualToFormalArgs();

		//functionDef child 2 eval statements from compound statement
		funcDef->GetChild(2)->Visit_Eval(this);
		
		result = m_returnVal;
		m_endVisit = false;
		m_insideFuncDef = false;
	}

	return result;
}

CAddition::CAddition(STNode* arg1, STNode* arg2) : CExpression(NodeType::EXP_ADDITION, 2, arg1, arg2) {}
CAddition::~CAddition() {}

double CAddition::Visit_Eval(STNode* parent) {

	return GetChild(0)->Visit_Eval(this) + GetChild(1)->Visit_Eval(this);
}

CSubtraction::CSubtraction(STNode* arg1, STNode* arg2) : CExpression(NodeType::EXP_SUBTRACTION, 2, arg1, arg2) {}
CSubtraction::~CSubtraction() {}

double CSubtraction::Visit_Eval(STNode* parent) {

	return GetChild(0)->Visit_Eval(this) - GetChild(1)->Visit_Eval(this);
}

CMultiplication::CMultiplication(STNode* arg1, STNode* arg2) : CExpression(NodeType::EXP_MULTIPLICATION, 2, arg1, arg2) {}
CMultiplication::~CMultiplication() {}

double CMultiplication::Visit_Eval(STNode* parent) {

	return GetChild(0)->Visit_Eval(this) * GetChild(1)->Visit_Eval(this);
}

CDivision::CDivision(STNode* arg1, STNode* arg2) : CExpression(NodeType::EXP_DIVISION, 2, arg1, arg2) {}
CDivision::~CDivision() {}

double CDivision::Visit_Eval(STNode* parent) {
	double num2 = GetChild(1)->Visit_Eval(this);
	if (num2 == 0) {
		cout << "second arg is 0, there could be no division" << endl;
		return 0;
	}
	return GetChild(0)->Visit_Eval(this) / num2;
}

CUnaryPlus::CUnaryPlus(STNode* arg) : CExpression(NodeType::EXP_UNARYPLUS, 1, arg) {}
CUnaryPlus::~CUnaryPlus() {}

double CUnaryPlus::Visit_Eval(STNode* parent) {

	return GetChild(0)->Visit_Eval(this);
}

CUnaryMinus::CUnaryMinus(STNode* arg) : CExpression(NodeType::EXP_UNARYMINUS, 1, arg) {}
CUnaryMinus::~CUnaryMinus() {}

double CUnaryMinus::Visit_Eval(STNode* parent) {

	return -GetChild(0)->Visit_Eval(this);
}

CAssignment::CAssignment(STNode* id, STNode* arg) : CExpression(NodeType::EXP_ASSIGNMENT, 2, id, arg) {}
CAssignment::~CAssignment() {}

double CAssignment::Visit_Eval(STNode* parent) {

	//we have two children
	//0 is the identifier
	//1 is the value
	CExpIdentifier* id = (CExpIdentifier*)GetChild(0);

	string name = _strdup(((CIDENTIFIER*)id)->m_name.c_str());

	//pass the id to the symboltable
	Symbol* symbol = g_symbolTable.GetIdentifier(name);

	symbol->value = GetChild(1)->Visit_Eval(this);

	if(m_insideFuncDef != true) {
		cout << symbol->m_name << "=" << symbol->value << endl;
	}

	return symbol->value;
}

CExpAnd::CExpAnd(STNode* arg1, STNode* arg2) : CExpression(NodeType::EXP_AND, 2, arg1, arg2) {}
CExpAnd::~CExpAnd() {}

double CExpAnd::Visit_Eval(STNode* parent) {
	return GetChild(0)->Visit_Eval(this) && GetChild(1)->Visit_Eval(this);
}

CExpOr::CExpOr(STNode* arg1, STNode* arg2) : CExpression(NodeType::EXP_OR, 2, arg1, arg2) {}
CExpOr::~CExpOr() {}

double CExpOr::Visit_Eval(STNode* parent) {
	return GetChild(0)->Visit_Eval(this) || GetChild(1)->Visit_Eval(this);
}

CExpNot::CExpNot(STNode* arg) : CExpression(NodeType::EXP_NOT) {}
CExpNot::~CExpNot() {}

double CExpNot::Visit_Eval(STNode* parent) {
	return !GetChild(0)->Visit_Eval(this);
}

CExpEqual::CExpEqual(STNode* arg1, STNode* arg2) : CExpression(NodeType::EXP_EQUAL, 2, arg1, arg2) {}
CExpEqual::~CExpEqual() {}

double CExpEqual::Visit_Eval(STNode* parent) {
	if (GetChild(0)->Visit_Eval(this) == GetChild(1)->Visit_Eval(this)) {
		return 1;
	}
	else {
		return 0;
	}
}

CExpNEqual::CExpNEqual(STNode* arg1, STNode* arg2) : CExpression(NodeType::EXP_NEQUAL, 2, arg1, arg2) {}
CExpNEqual::~CExpNEqual() {}

double CExpNEqual::Visit_Eval(STNode* parent) {
	if (GetChild(0)->Visit_Eval(this) != GetChild(1)->Visit_Eval(this)) {
		return 1;
	}
	else {
		return 0;
	}
}

CExpGT::CExpGT(STNode* arg1, STNode* arg2) : CExpression(NodeType::EXP_GT, 2, arg1, arg2) {}
CExpGT::~CExpGT() {}

double CExpGT::Visit_Eval(STNode* parent) {
	if (GetChild(0)->Visit_Eval(this) > GetChild(1)->Visit_Eval(this)) {
		return 1;
	}
	else {
		return 0;
	}
}

CExpLT::CExpLT(STNode* arg1, STNode* arg2) : CExpression(NodeType::EXP_LT, 2, arg1, arg2) {}
CExpLT::~CExpLT() {}

double CExpLT::Visit_Eval(STNode* parent) {
	if (GetChild(0)->Visit_Eval(this) < GetChild(1)->Visit_Eval(this)) {
		return 1;
	}
	else {
		return 0;
	}
}

CExpGTE::CExpGTE(STNode* arg1, STNode* arg2) : CExpression(NodeType::EXP_GTE, 2, arg1, arg2) {}
CExpGTE::~CExpGTE() {}

double CExpGTE::Visit_Eval(STNode* parent) {
	if (GetChild(0)->Visit_Eval(this) >= GetChild(1)->Visit_Eval(this)) {
		return 1;
	}
	else {
		return 0;
	}
}

CExpLTE::CExpLTE(STNode* arg1, STNode* arg2) : CExpression(NodeType::EXP_LTE, 2, arg1, arg2) {}
CExpLTE::~CExpLTE() {}

double CExpLTE::Visit_Eval(STNode* parent) {
	if (GetChild(0)->Visit_Eval(this) <= GetChild(1)->Visit_Eval(this)) {
		return 1;
	}
	else {
		return 0;
	}
}

CNUMBER::CNUMBER(char* text, double num) : CExpression(NodeType::EXP_NUMBER) {
	m_name = text;
	m_number = num;
	m_graphvizLabel.append("_" + std::to_string(m_number));
}
CNUMBER::~CNUMBER() {}

double CNUMBER::Visit_Eval(STNode* parent) {
	return m_number;
}

CExpIdentifier::CExpIdentifier(STNode* id) : CExpression(NodeType::EXP_IDENTIFIER) {
	m_name = _strdup(((CIDENTIFIER*)id)->m_name.c_str());
	//m_graphvizLabel.append("_" + m_name);
}
CExpIdentifier::~CExpIdentifier() {}

double CExpIdentifier::Visit_Eval(STNode* parent) {
	return g_symbolTable.GetIdentifier(m_name)->value;
}

CIDENTIFIER::CIDENTIFIER(const char* text) : CExpression(NodeType::IDENTIFIER) {
	m_name = string(text);
	m_number = 0;
	m_graphvizLabel.append("_" + m_name);
}
CIDENTIFIER::~CIDENTIFIER() {}

double CIDENTIFIER::Visit_Eval(STNode* parent) {
	return 0;
}

void CCompileUnit::Visit_SyntaxTreePrinter(ofstream* dotFile, STNode* parent) {
	list<STNode*>::iterator it;

	if (parent == nullptr) {
		(*dotFile) << "digraph G{ \n";
		for (it = m_children->begin(); it != m_children->end(); it++) {
			(*it)->Visit_SyntaxTreePrinter(dotFile, this);
		}
		(*dotFile) << "}";
		dotFile->close();
	}
	else {
		STNode::Visit_SyntaxTreePrinter(dotFile, parent);
	}
}

