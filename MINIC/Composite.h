#pragma once
#include <fstream>
#include <string>
#include <list>
#include <cstdarg>
using namespace std;

class STNode;
class CIDENTIFIER;
class CExpression;

extern STNode* g_root;
extern const char* g_nodeTypeLabels[];

typedef enum nodeType {
	COMPILEUNIT, STATEMENT_EXP, EMPTY_STATEMENT,
	FUNCTIONDEF, FARGS, ARGS,
	COMPOUND_STATEMENT, STATEMENT_LIST,
	IF_STATEMENT, WHILE_STATEMENT,
	BREAK_STATEMENT, RETURN_STATEMENT, FUNCTION_CALL,
	EXP_NUMBER, EXP_IDENTIFIER, IDENTIFIER,
	EXP_ADDITION, EXP_SUBTRACTION,
	EXP_MULTIPLICATION, EXP_DIVISION,
	EXP_UNARYPLUS, EXP_UNARYMINUS,
	EXP_ASSIGNMENT, EXP_AND, EXP_OR, EXP_NOT,
	EXP_NEQUAL, EXP_EQUAL, EXP_GT, EXP_LT, EXP_GTE, EXP_LTE
}NodeType;

class STNode {
public:
	STNode(NodeType, int, ...);
	virtual ~STNode();

	NodeType GetNodeType();
	string GetGraphVizLabel();
	STNode* GetChild(int index);
	STNode* GetParent(int index = 0);

	virtual void Visit_SyntaxTreePrinter(ofstream* dotFile, STNode* parent);
	virtual double Visit_Eval(STNode* parent);

	friend void GetFArgs(STNode* node, list<CIDENTIFIER*>* fargs);
	friend void GetArgs(STNode* node, list<CExpression*>* args);

protected:
	static bool m_endVisit;
	static double m_returnVal;
	static bool m_insideFuncDef;

	//type of node
	NodeType m_nodeType;

	//graphiz node label
	string m_graphvizLabel;

	//node serial number
	int m_serial;

	//counter for making node serial unique 
	static int m_serialCounter;

	//children nodes list
	list<STNode*>* m_children;

	//parent nodes list
	list<STNode*>* m_parents;

};
