#include "Composite.h"

STNode* g_root = nullptr;

bool STNode::m_endVisit = false;

bool STNode::m_insideFuncDef = false;

double STNode::m_returnVal = 0;

int STNode::m_serialCounter = 0;

const char* g_nodeTypeLabels[] = {
	"COMPILEUNIT", "STATEMENT_EXP", "EMPTY_STATEMENT", 
	"FUNCTIONDEF", "FARGS", "ARGS",
	"COMPOUND_STATEMENT", "STATEMENT_LIST",
	"IF_STATEMENT", "WHILE_STATEMENT",
	"BREAK_STATEMENT", "RETURN_STATEMENT", "FUNCTION_CALL",
	"EXP_NUMBER", "EXP_IDENTIFIER", "IDENTIFIER",
	"EXP_ADDITION", "EXP_SUBTRACTION",
	"EXP_MULTIPLICATION", "EXP_DIVISION",
	"EXP_UNARYPLUS", "EXP_UNARYMINUS",
	"EXP_ASSIGNMENT", "EXP_AND", "EXP_OR", "EXP_NOT",
	"EXP_NEQUAL", "EXP_EQUAL", "EXP_GT", "EXP_LT", "EXP_GTE", "EXP_LTE"
};

STNode::STNode(NodeType type, int num, ...) : m_nodeType{ type } {
	va_list arguments;
	STNode* child;

	//initialize va_list
	va_start(arguments, num);

	//m_nodeType = type;
	m_serial = m_serialCounter++;
	m_children = new list<STNode*>;
	m_parents = new list<STNode*>;
	m_graphvizLabel = _strdup(g_nodeTypeLabels[m_nodeType]);
	m_graphvizLabel.append("_" + std::to_string(m_serial));

	for (int i = 0; i < num; i++) {
		child = va_arg(arguments, STNode*);
		m_children->push_back(child);
		child->m_parents->push_back(this);
	}

	//free memory for va_list
	va_end(arguments);
}

STNode::~STNode() {
	list<STNode*>::iterator it;
	for (it = m_children->begin(); it != m_children->end(); it++) {
		delete* it;
	}
	delete m_children;
	delete m_parents;
}

NodeType STNode::GetNodeType() {
	return m_nodeType;
}

string STNode::GetGraphVizLabel() {
	return m_graphvizLabel;
}

STNode* STNode::GetChild(int index) {
	list<STNode*>::iterator it = m_children->begin();
	advance(it, index);
	return *it;
}

STNode* STNode::GetParent(int index) {
	list<STNode*>::iterator it = m_parents->begin();
	advance(it, index);
	return *it;
}

void STNode::Visit_SyntaxTreePrinter(ofstream* dotFile, STNode* parent) {
	list<STNode*>::iterator it;

	(*dotFile) << "\"" << parent->GetGraphVizLabel() << "\"->\"" << GetGraphVizLabel() << "\";\n";

	for (it = m_children->begin(); it != m_children->end(); it++) {
		(*it)->Visit_SyntaxTreePrinter(dotFile, this);
	}
}

double STNode::Visit_Eval(STNode* parent) {
	list<STNode*>::iterator it;
	double result = 0;

	for (it = m_children->begin(); it != m_children->end(); it++) {

		if ((*it)->GetNodeType() != FUNCTIONDEF) {
			result = (*it)->Visit_Eval(parent);

			if (m_endVisit == true) {
				break;
			}
		}
	}
	return result;
}

